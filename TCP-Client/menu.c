// Cyril Kamal - Dec 2021
//
// This file contains the functionality that the user can interact with.
// The menu accepts input from the user, validates input, and forwards input to the RESTful API

#include "menu.h"
#include "RESTfulAPI.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


void printMainMenuOptions()
{
	fputs("~~~~~~~~~~~~~~~~~~~~ REST API ~~~~~~~~~~~~~~~~~~~\n", stdout);
	fputs("To choose a function, enter its letter label:\n\n", stdout);
	fputs("a) Show all posts\n", stdout);
	fputs("b) Search for a post by ID\n", stdout);
	fputs("c) Update a post\n", stdout);
	fputs("d) Create a post\n", stdout);
	fputs("e) Delete a post\n", stdout);
	fputs("f) Quit\n", stdout);
	fputs("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n", stdout);
	fputs("Option: ", stdout);
}

// This function is the first step to processing user interaction with the program
void executeUserOptionChoice(SOCKET peer_socket)
{
	char user_input[MAX_USER_INPUT];
	fgets(user_input, MAX_USER_INPUT, stdin);
	user_input[strcspn(user_input, "\n")] = 0;
	char serverResponse[MAX_BUFFER];
	char input[MAX_USER_INPUT];
	//memset(input, '\0', MAX_USER_INPUT);
	char postAuthor[MAX_USER_INPUT];
	//memset(input, '\0', MAX_USER_INPUT);
	char postTopic[MAX_USER_INPUT];
	//memset(input, '\0', MAX_USER_INPUT);
	char postPosting[MAX_USER_INPUT];
	//memset(input, '\0', MAX_USER_INPUT);

	if (!strcmp("a", user_input)) // GET ALL POSTS
	{
		GetAllPosts(peer_socket);
	}
	else if (!strcmp("b", user_input)) // GET SPECIFIC POST
	{
		GetSpecificPost(peer_socket);

	}
	else if (!strcmp("c", user_input)) // UPDATE AN EXISTING POST
	{
		UpdatePost(peer_socket);
	}
	else if (!strcmp("d", user_input)) // CREATE A POST
	{
		CreatePost(peer_socket);
	}
	else if (!strcmp("e", user_input)) // DELETE POST
	{
		DeletePost(peer_socket);
	}
	else if (!strcmp("f", user_input)) //QUIT
	{
		printf("Closing the connection...\n");
		CloseSocketConnection(peer_socket);
		ShutdownWindowsSockets();
		exit(1);
	}
	else
	{
		printf("\nNot a valid input.\n");
		return;
	}
	
}

void GetAllPosts(SOCKET peer_socket)
{
	char serverResponse[MAX_BUFFER];
	GETposts(peer_socket);
	strcpy(serverResponse, receiveMessage(peer_socket));
	if (IsRequestSuccessful(serverResponse))
	{
		processGETpostsResponse(serverResponse);
	}
	else
	{
		printf("\nDatabase is empty\n");
	}
}

void GetSpecificPost(SOCKET peer_socket)
{
	char serverResponse[MAX_BUFFER];
	char input[MAX_USER_INPUT];
	memset(input, '\0', MAX_USER_INPUT);

	printf("\nEnter the postID you are searching for: ");
	fgets(input, MAX_USER_INPUT, stdin);
	input[strcspn(input, "\n")] = 0;
	int inputLen = strlen(input);
	if (isInputAnInteger(input, inputLen))
	{
		int postID = atoi(input);
		GETpost(peer_socket, postID);
		strcpy(serverResponse, receiveMessage(peer_socket));
		if (IsRequestSuccessful(serverResponse))
		{
			processGETpostResponse(serverResponse);
		}
		else
		{
			printf("\nPost ID %d does not exist in database\n", postID);
		}
	}
	else
	{
		printf("\nInput not a valid integer number.\n");
	}
}

void UpdatePost(SOCKET peer_socket)
{
	char serverResponse[MAX_BUFFER];

	// Initialize buffers
	char input[MAX_USER_INPUT];
	memset(input, '\0', MAX_USER_INPUT);
	char postAuthor[MAX_USER_INPUT];
	memset(input, '\0', MAX_USER_INPUT);
	char postTopic[MAX_USER_INPUT];
	memset(input, '\0', MAX_USER_INPUT);
	char postPosting[MAX_USER_INPUT];
	memset(input, '\0', MAX_USER_INPUT);

	// Grab post num from user
	printf("\nEnter the postID you are searching for: ");
	fgets(input, MAX_USER_INPUT, stdin);
	input[strcspn(input, "\n")] = 0;
	int inputLen = strlen(input);

	if (isInputAnInteger(input, inputLen)) // Check if user input is indeed a number
	{
		int postID = atoi(input);
		GETpost(peer_socket, postID);
		strcpy(serverResponse, receiveMessage(peer_socket));

		if (IsRequestSuccessful(serverResponse)) // Check if a post with that post number is in the database
		{
			printf("\nEnter new author name: ");
			fgets(postAuthor, MAX_USER_INPUT - 1, stdin);
			postAuthor[strcspn(postAuthor, "\n")] = '\0';

			printf("\nEnter new topic: ");
			fgets(postTopic, MAX_USER_INPUT - 1, stdin);
			postTopic[strcspn(postTopic, "\n")] = '\0';

			printf("\nEnter new posting: ");
			fgets(postPosting, MAX_USER_INPUT - 1, stdin);
			postPosting[strcspn(postPosting, "\n")] = '\0';

			PUT(peer_socket, postID, postAuthor, postTopic, postPosting); // send request
			strcpy(serverResponse, receiveMessage(peer_socket)); // catch response
			
			if (IsRequestSuccessful(serverResponse))
			{
				GETpost(peer_socket, postID); // Get the new post from the database to show the user
				strcpy(serverResponse, receiveMessage(peer_socket)); // catch response
				if (IsRequestSuccessful(serverResponse))
				{
					processGETpostResponse(serverResponse);
				}
				else
				{
					printf("\nUpdate not successful\n");
				}
			}
			else
			{
				printf("\nUpdate not successful\n");
			}
		}
		else
		{
			printf("\nPost ID %d does not exist in database\n", postID);
		}
	}
	else
	{
		printf("\nInput not a valid integer number.\n");
	}
}

void CreatePost(SOCKET peer_socket)
{
	// Initialize buffers
	char serverResponse[MAX_BUFFER];
	char postAuthor[MAX_USER_INPUT];
	char postTopic[MAX_USER_INPUT];
	char postPosting[MAX_USER_INPUT];

	printf("\nEnter new author name: ");
	fgets(postAuthor, MAX_USER_INPUT - 1, stdin);
	postAuthor[strcspn(postAuthor, "\n")] = '\0';

	printf("\nEnter new topic: ");
	fgets(postTopic, MAX_USER_INPUT - 1, stdin);
	postTopic[strcspn(postTopic, "\n")] = '\0';

	printf("\nEnter new posting: ");
	fgets(postPosting, MAX_USER_INPUT - 1, stdin);
	postPosting[strcspn(postPosting, "\n")] = '\0';

	POST(peer_socket, postAuthor, postTopic, postPosting); // send request
	strcpy(serverResponse, receiveMessage(peer_socket)); // catch response

	if (IsRequestSuccessful(serverResponse))
	{
		printf("\nPost successful. \n");
	}
	else
	{
		printf("\nPost failed.\n");
	}
}

void DeletePost(SOCKET peer_socket)
{
	// Initialize buffers
	char serverResponse[MAX_BUFFER];
	char input[MAX_USER_INPUT];

	printf("\nEnter the postID you are searching for: ");
	fgets(input, MAX_USER_INPUT, stdin);
	input[strcspn(input, "\n")] = 0;

	int inputLen = strlen(input);

	if (isInputAnInteger(input, inputLen))
	{
		int postID = atoi(input);
		GETpost(peer_socket, postID); // Check if post with postID is present in the database
		strcpy(serverResponse, receiveMessage(peer_socket)); // catch response

		if (IsRequestSuccessful(serverResponse))
		{
			DELETE_POST(peer_socket, postID); // send message
			strcpy(serverResponse, receiveMessage(peer_socket)); // catch response

			if (IsRequestSuccessful(serverResponse))
			{
				printf("\nDeletion successful.\n");
			}
			else
			{
				printf("\nDeletion not successful\n");
			}
		}
		else
		{
			printf("\nPost ID %d does not exist in database\n", postID);
		}
	}
	else
	{
		printf("\nInput not a valid integer number.\n");
	}
}

bool isInputAnInteger(char* input, int inputLen)
{
	if (input[0] == '0')
		return false;
	for (int i = 0; i < inputLen; i++) // compare ASCII values of characters to see if they are numbers.
	{
		if (input[i] < 48 || input[i] > 57)
			return false;
	}
	return true;
}