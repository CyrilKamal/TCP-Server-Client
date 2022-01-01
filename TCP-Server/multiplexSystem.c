#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include "NetworkingFunctions.h"
#include "multiplexSystem.h"
#include "Posts.h"


// This function runs in an infinite loop managing connections to the server
SOCKET WaitForAndAcceptAndHandleMultiplexedConnections(SOCKET socket_listen, POSTS* postList)
{
	fd_set master;
	FD_ZERO(&master);
	FD_SET(socket_listen, &master);
	SOCKET max_socket = socket_listen;


	while (1)
	{
		fd_set reads;
		reads = master;
		if (select((int)(max_socket + 1), &reads, 0, 0, 0) < 0)
		{
			fprintf(stderr, "select() failed exiting with an error of (%d)\n", WSAGetLastError());
			exit(1);
		}
		SOCKET i;
		for (i = 1; i <= max_socket; ++i) //handle a new socket request with an accept
		{
			if (FD_ISSET(i, &reads))
			{
				if (i == socket_listen)
				{
					struct sockaddr_storage client_address;
					socklen_t client_len = sizeof(client_address);
					SOCKET socket_client = accept(socket_listen, (struct sockaddr*)&client_address, &client_len);
					if ((socket_client) == INVALID_SOCKET)
					{
						fprintf(stderr, "accept() failed. (%d)\n", WSAGetLastError());
						exit(1);
					}

					FD_SET(socket_client, &master);
					if (socket_client > max_socket)
						max_socket = socket_client;

					char address_buffer[DISPLAYBUFFERSIZE];
					getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
					printf("New connection from %s\n", address_buffer);
				}
				else
				{
					char read[SENDBUFFERSIZE];
					int bytes_received = recv(i, read, SENDBUFFERSIZE, 0);
					if (bytes_received < 1)
					{
						FD_CLR(i, &master);
						CloseSocketConnection(i);
						continue;
					}

					printf("%.*s", bytes_received, read);
					printf("received message (size: %d) from %I64d\n", bytes_received, i);
					ProcessRequest(i, postList, read);
				}
			}
		}
	}
}


// This function is the first step at analyzing incoming HTML messages that the server receives.
// It forwards messages for further processing based on their header information (GET, POST, DELETE, etc)
// 
// Every query that alters the database is immediately followed up with a save to disk!
void ProcessRequest(SOCKET socket_listen, POSTS* postList, char* message)
{
	char* messageCopy = (char*)malloc(strlen(message));
	strcpy(messageCopy, message);
	char* request = strtok(messageCopy, "\r\n");

	if (strstr(request, "GET /posts/") != NULL)
	{
		ProcessGetPost(socket_listen, postList, message);
	}
	else if (strstr(request, "GET /posts") != NULL)
	{
		ProcessGetPosts(socket_listen, postList);
	}
	else if (strstr(request, "POST") != NULL)
	{
		ProcessPost(socket_listen, postList, message);
		savePostsToDrive(postList);
	}
	else if (strstr(request, "PUT") != NULL)
	{
		ProcessPut(socket_listen, postList, message);
		savePostsToDrive(postList);
	}
	else if (strstr(request, "DELETE") != NULL)
	{
		ProcessDelete(socket_listen, postList, message);
		savePostsToDrive(postList);
	}
	else
	{
		SendErrorCode(socket_listen);
	}
}


// This function handles GET requests for specific posts.
// It finds the post, bundles it up, and sends it back to the client.
void ProcessGetPost(SOCKET socket_listen, POSTS* postList, char* request)
{
	int postNum;
	sscanf(request, "%*[^0123456789]%d", &postNum);
	
	POST* requestedPost = searchPostsByID(postList, postNum);

	if (requestedPost == NULL)
	{
		SendErrorCode(socket_listen);
	}
	else
	{
		char message[SENDBUFFERSIZE];
		memset(message, '\0', SENDBUFFERSIZE);

		char* postString = stringifyPost(requestedPost);
		int contentLength = strlen(postString);

		const char* header =
			"HTTP/1.1 200 OK\r\n"
			"content-type: application/json\r\n"
			"Connection: close\r\n"
			"Content-Length: ";

		sprintf(message, "%s%d\n\n%s\0\0", header, contentLength, postString);

		int sent = send(socket_listen, message, strlen(message), 0);
		if (sent == 0)
		{
			fprintf(stderr, "send failed\n");
			exit(1);
		}
	}
}


// This funcitons handles GET requests for all posts.
// It bundles the database up as one string and sends it back to the user.
void ProcessGetPosts(SOCKET socket_listen, POSTS* postList)
{
	int numOfPosts = getPostCount(postList);
	if (postList->posts == NULL)
	{
		SendErrorCode(socket_listen);
		return;
	}

	char message[SENDBUFFERSIZE];
	memset(message, '\0', SENDBUFFERSIZE);

	char postListString[SENDBUFFERSIZE];
	strcpy(postListString, stringifyPostings(postList));
	int contentLength = strlen(postListString);

	const char* header =
		"HTTP/1.1 200 OK\r\n"
		"content-type: application/json\r\n"
		"Connection: close\r\n"
		"Content-Length: ";

	sprintf(message, "%s%d\n\n%s\0\0", header, contentLength, postListString);

	int sent = send(socket_listen, message, strlen(message), 0);
	if (sent == 0)
	{
		fprintf(stderr, "send failed\n");
		exit(1);
	}
}


// This function handles PUT requests and updates the database accordingly.
void ProcessPut(SOCKET socket_listen, POSTS* postList, char* request)
{
	int postNum;
	sscanf(request, "%*[^0123456789]%d", &postNum);

	POST* updatedPost = searchPostsByID(postList, postNum);

	if (updatedPost == NULL)
	{
		SendErrorCode(socket_listen);
	}
	else
	{
		char* update = strstr(request, "\n\n"); // find the message content after the http header information
		update = update + 2;
		if (update != NULL)
		{
			char* token;
			token = strtok(update, "\n");
			strcpy(updatedPost->author, token);
			token = strtok(NULL, "\n");
			strcpy(updatedPost->topic, token);
			token = strtok(NULL, "\n");
			strcpy(updatedPost->posting, token);
			token = strtok(NULL, "\n");

			SendSuccessCode(socket_listen);
		}
		else
			SendErrorCode(socket_listen);
	}
}


// This function handles POST request by parsing user data sent and forming post 
// structs that can be added to the database.
void ProcessPost(SOCKET socket_listen, POSTS* postList, char* request)
{
	char* message = strstr(request, "\n\n");
	message = message + 2;

	char* token;

	token = strtok(message, "\n");
	char nameBuffer[MAX_BUFFER];
	strcpy(nameBuffer, token);

	token = strtok(NULL, "\n");
	char topicBuffer[MAX_BUFFER];
	strcpy(topicBuffer, token);

	token = strtok(NULL, "\n");
	char postingBuffer[MAX_BUFFER];
	strcpy(postingBuffer, token);

	int newPostID = generatePostID(postList);
	POST* newPost = createPost(newPostID, nameBuffer, topicBuffer, postingBuffer);
	addPostToPostList(postList, newPost);

	if (searchPostsByID(postList, newPostID) != NULL)
		SendSuccessCode(socket_listen);
	else
		SendErrorCode(socket_listen);
}


// This function handles DELETE requests by matching the postID numbers and removing them from the database.
void ProcessDelete(SOCKET socket_listen, POSTS* postList, char* request)
{
	int postNum;
	sscanf(request, "%*[^0123456789]%d", &postNum);

	POST* requestedPost = searchPostsByID(postList, postNum);

	if (requestedPost == NULL)
	{
		SendErrorCode(socket_listen);
	}
	else
	{
		removePostFromPostList(postList, postNum);
		if (searchPostsByID(postList, postNum) == NULL)
			SendSuccessCode(socket_listen);
		else
			SendErrorCode(socket_listen);
	}
}


// This function sends a general error code in the event that the server cannot comply with requests.
void SendErrorCode(SOCKET socket_listen)
{
	const char* header =
		"HTTP/1.1 500 OK\r\n"
		"content-type: application/json\r\n"
		"Connection: close\r\n";

	int sent = send(socket_listen, header, strlen(header), 0);
	if (sent == 0)
	{
		fprintf(stderr, "send failed\n");
		exit(1);
	}
	return;
}


// This function sends a general success code in the event that a client request requires no output.
void SendSuccessCode(SOCKET socket_listen)
{
	const char* header =
		"HTTP/1.1 200 OK\r\n"
		"content-type: application/json\r\n"
		"Connection: close\r\n";

	int sent = send(socket_listen, header, strlen(header), 0);
	if (sent == 0)
	{
		fprintf(stderr, "send failed\n");
		exit(1);
	}
	return;
}