// Cyril Kamal - Dec 2021
//
// post structs are how the database views database data.


#define _CRT_SECURE_NO_WARNINGS

#include "Post.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


POST* createPost(int postID, char* author, char* topic, char* posting)
{
	POST* newPost = (POST*)malloc(sizeof(POST));

	newPost->postID = postID;

	newPost->author = (char*)malloc(sizeof(strlen(author) + 1));
	strcpy(newPost->author, author);

	newPost->topic = (char*)malloc(sizeof(strlen(topic) + 1));
	strcpy(newPost->topic, topic);

	newPost->posting = (char*)malloc(sizeof(strlen(topic) + 1));
	strcpy(newPost->posting, posting);

	return newPost;
}

bool comparePosts(POST* post1, POST* post2)
{
	if (!strcmp(post1->author, post2->author) && !strcmp(post1->topic, post2->topic) && !strcmp(post1->posting, post2->posting))
		return true;
	else
		return false;
}

void streamPrintPost(POST* post)
{
	printf("%s\n", post->author);
	printf("%s\n", post->topic);
	printf("%s\n", post->posting);
}

void disposePost(POST* post)
{
	free(post->author);
	free(post->topic);
	free(post->posting);
	free(post);
}