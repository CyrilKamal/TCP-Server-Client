

#include "RESTClient.h"
#include "NetworkingFunctions.h"
#include <stdio.h>


void GETposts(SOCKET peer_socket)
{
	char* REST = "GET /posts HTTP/1.1";
	char message[SENDBUFFERSIZE];
	memset(message, '\0', SENDBUFFERSIZE);

	const char* header =
		"content-type: application/json\r\n"
		"accept-encoding: gzip, defalte\r\n"
		"Connection: close\r\n";

	sprintf(message, "%s\r\n%s\n\0\0", REST, header);

	int sent = send(peer_socket, message, strlen(message), 0);
	if (sent == 0)
	{
		fprintf(stderr, "send failed\n");
		exit(1);
	}
}

void GETpost(SOCKET peer_socket, int postID)
{
	char* rest;
	sprintf(rest, "GET /posts/%d HTTP/1.1", postID);

	char message[SENDBUFFERSIZE];
	memset(message, '\0', SENDBUFFERSIZE);

	const char* header =
		"content-type: application/json\r\n"
		"accept-encoding: gzip, defalte\r\n"
		"Connection: close\r\n";

	sprintf(message, "%s\r\n%s\n\0\0", rest, header);

	int sent = send(peer_socket, message, strlen(message), 0);
	if (sent == 0)
	{
		fprintf(stderr, "send failed\n");
		exit(1);
	}
}

void POST(SOCKET peer_socket, char* post)
{
	char* rest;
	sprintf(rest, "POST /posts HTTP/1.1");

	char message[SENDBUFFERSIZE];
	memset(message, '\0', SENDBUFFERSIZE);

	const char* header =
		"content-type: application/json\r\n"
		"accept-encoding: gzip, defalte\r\n"
		"Connection: close\r\n";

	sprintf(message, "%s\r\n%s\r\n\n%s\n\0\0", rest, header, post);

	int sent = send(peer_socket, message, strlen(message), 0);
	if (sent == 0)
	{
		fprintf(stderr, "send failed\n");
		exit(1);
	}
}

void PUT(SOCKET peer_socket, int postID, char* update)
{
	char* rest;
	sprintf(rest, "POST /posts/%d HTTP/1.1", postID);

	char message[SENDBUFFERSIZE];
	memset(message, '\0', SENDBUFFERSIZE);

	const char* header =
		"content-type: application/json\r\n"
		"accept-encoding: gzip, defalte\r\n"
		"Connection: close\r\n";

	sprintf(message, "%s\r\n%s\r\n\n%s\n\0\0", rest, header, update);

	int sent = send(peer_socket, message, strlen(message), 0);
	if (sent == 0)
	{
		fprintf(stderr, "send failed\n");
		exit(1);
	}
}

void DELETE_POST(SOCKET peer_socket, int postID)
{
	char* rest;
	sprintf(rest, "GET /posts/%d HTTP/1.1", postID);

	char message[SENDBUFFERSIZE];
	memset(message, '\0', SENDBUFFERSIZE);

	const char* header =
		"content-type: application/json\r\n"
		"accept-encoding: gzip, defalte\r\n"
		"Connection: close\r\n";

	sprintf(message, "%s\r\n%s\n\0\0", rest, header);

	int sent = send(peer_socket, message, strlen(message), 0);
	if (sent == 0)
	{
		fprintf(stderr, "send failed\n");
		exit(1);
	}
}