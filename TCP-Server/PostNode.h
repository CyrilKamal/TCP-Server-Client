// Cyril Kamal - Dec 2021
//
// header file for PostNode.c

#pragma once

#include "Post.h"

typedef struct postnode
{
	POST* NodeData;
	struct postnode* next;
}POST_NODE;

POST_NODE* createPostNode(POST*);
void setPostNodeNextPostNode(POST_NODE*, POST_NODE*);
POST_NODE* getPostNodeNextPostNode(POST_NODE*);
void disposePostNode(POST_NODE*);