// Cyril Kamal - Dec 2021
//
// the posts struct represents the entire database as a linked list
// which the server can use to fulfill user requests.

#define _CRT_SECURE_NO_WARNINGS

#include "Posts.h"
#include "PostNode.h"
#include "Post.h"
#include "multiplexSystem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

POSTS* createPostList()
{
	POSTS* postList = (POSTS*)malloc(sizeof(POSTS));
	postList = NULL;
	return &postList;
}

void addPostToPostList(POSTS* postList, POST* newPost)
{
	POST_NODE* newNode = createPostNode(newPost);

	if (postList->posts == NULL) // No posts yet
	{
		setPostNodeNextPostNode(newNode, postList->posts);
		postList->posts = newNode;
	}
	else
	{
		POST_NODE* current = postList->posts;
		while (getPostNodeNextPostNode(current) != NULL)
		{
			current = getPostNodeNextPostNode(current);
		}
		setPostNodeNextPostNode(current, newNode);
	}
}

void removePostFromPostList(POSTS* postList, int postNum)
{
	if (postList->posts == NULL) // No posts
	{
		return;
	}
	else
	{
		POST_NODE* current = postList->posts;
		POST_NODE* prev = NULL;
		while (current->NodeData->postID != postNum)
		{
			prev = current;
			current = current->next;
		}

		prev->next = current->next;
		return;
	}
}

POST* searchPostsByID(POSTS* postList, int postID)
{
	if (postList == NULL)
	{
		return NULL;
	}
	else
	{
		POST_NODE* current = postList->posts;
		while (current != NULL)
		{
			if (current->NodeData->postID == postID)
				return current->NodeData;
			else
				current = current->next;
		}

		return NULL;
	}
}

// Export all posts in memory to a text file.
void savePostsToDrive(POSTS* postList) 
{
	FILE* fp;
	if (((fp = fopen(FILE_NAME, "w+")) == NULL))
	{
		exit(1);
	}
	else
	{
		POST_NODE* current = postList->posts;
		while (current != NULL)
		{
			fprintf(fp, "%d\n", current->NodeData->postID);
			fprintf(fp, "%s\n", current->NodeData->author);
			fprintf(fp, "%s\n", current->NodeData->topic);
			fprintf(fp, "%s\n", current->NodeData->posting);
			current = getPostNodeNextPostNode(current);
		}
		fclose(fp);
	}
}

// If no database can be located on the drive, a blank file is created for server use.
POSTS* loadPostsFromDrive(POSTS* postList)
{
	FILE* fp;
	if ((fp = fopen(FILE_NAME, "r")) == NULL)
	{
		return postList;
	}
	else
	{
		char tempID[MAX_BUFFER];
		char author[MAX_BUFFER];
		char topic[MAX_BUFFER];
		char posting[MAX_BUFFER];

		int postID;
		POST* newPost;

		while (fgets(tempID, MAX_BUFFER, fp) != NULL)
		{
			postID = atoi(tempID);
			fgets(author, MAX_BUFFER, fp);
			author[strlen(author) - 1] = '\0';
			fgets(topic, MAX_BUFFER, fp);
			topic[strlen(topic) - 1] = '\0';
			fgets(posting, MAX_BUFFER, fp);
			posting[strlen(posting) - 1] = '\0';
			newPost = createPost(postID, author, topic, posting);
			addPostToPostList(postList, newPost);
		}
		fclose(fp);
	}

	return postList;
}

int getPostCount(POSTS* postList)
{
	if (postList->posts == NULL)
	{
		return 0;
	}
	else
	{
		POST_NODE* current = postList->posts;
		int numOfPosts = 0;
		while (current != NULL)
		{
			numOfPosts++;
			current = getPostNodeNextPostNode(current);
		}
		return numOfPosts;
	}
}

// When a user sends a POST request, a new postID must be generated and be UNIQUE
int generatePostID(POSTS* postList)
{
	if (postList->posts == NULL)
	{
		return 1;
	}
	else
	{
		POST_NODE* current = postList->posts;
		int newPostNumber = 0;
		while (current != NULL)
		{
			if (current->NodeData->postID >= newPostNumber)
			{
				newPostNumber = current->NodeData->postID + 1;
			}
			current = current->next;
		}
		return newPostNumber;
	}
}

// This function calls the stringifyPost function for each post in the linked list.
char* stringifyPostings(POSTS* postList)
{
	char message[SENDBUFFERSIZE];
	memset(message, '\0', SENDBUFFERSIZE);

	POST_NODE* current = postList->posts;
	while (current != NULL)
	{
		strcat(message, stringifyPost(current->NodeData));
		current = current->next;
	}

	return message;
}

// This function takes the attributes of a Post struct and concatenates them into a single string
// that can be sent along with an HTML message.
char* stringifyPost(POST* post)
{
	char stringifiedPost[SENDBUFFERSIZE];
	memset(stringifiedPost, '\0', SENDBUFFERSIZE);

	char postID[BUFFER];
	memset(postID, '\0', BUFFER);
	sprintf(postID, "%d", post->postID);
	strcat(stringifiedPost, postID);
	strcat(stringifiedPost, "\n");
	strcat(stringifiedPost, post->author);
	strcat(stringifiedPost, "\n");
	strcat(stringifiedPost, post->topic);
	strcat(stringifiedPost, "\n");
	strcat(stringifiedPost, post->posting);
	strcat(stringifiedPost, "\n");

	return stringifiedPost;
}