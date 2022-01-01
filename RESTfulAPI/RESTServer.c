
#define _CRT_SECURE_NO_WARNINGS

#include "RESTServer.h"
#include <stdio.h>
#include <string.h>

void ProcessRequest(char* message)
{
	char* messageCopy;
	strcpy(messageCopy, message);
	char* request = strtok(messageCopy, "\r\n");

	if (strstr(request, "GET /posts/") != NULL)
	{
		ProcessGetPost(request);
	}
	else if (strstr(request, "GET /posts") != NULL)
	{
		ProcessGetPosts();
	}
	else if (strstr(request, "POST") != NULL)
	{
		ProcessPost(message);
	}
	else if (strstr(request, "PUT") != NULL)
	{
		ProcessPut(message);
	}
	else if (strstr(request, "DELETE") != NULL)
	{
		ProcessDelete(message);
	}
	else
	{
		ProcessInvalidRequest();
	}
}

void ProcessGetPost(char* request)
{
	int postNum;
	sscanf(request, "%*[^0123456789]%d", &postNum);
}

void ProcessGetPosts()
{

}

void ProcessPost(char* request)
{

}

void ProcessPut(char* request)
{

}

void ProcessDelete(char* request)
{

}

void ProcessInvalidRequest()
{

}