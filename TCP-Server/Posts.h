// Cyril Kamal - Dec 2021
//
// Header file for Posts.c

#pragma once

#include "PostNode.h"
#include "Post.h"
#include <stdbool.h>

#define MAX_BUFFER		1000
#define FILE_NAME		"posts.txt"

typedef struct posts 
{
	POST_NODE* posts;
}POSTS;

POSTS* createPostList();
void addPostToPostList(POSTS*, POST*);
void removePostFromPostList(POSTS*, int);
POST* searchPostsByID(POSTS*, int);
void savePostsToDrive(POSTS*);
POSTS* loadPostsFromDrive(POSTS*);
int getPostCount(POSTS*);
int generatePostID(POSTS*);
char* stringifyPostings(POSTS*);
char* stringifyPost(POST* post);
