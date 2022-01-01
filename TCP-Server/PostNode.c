// Cyril Kamal - Dec 2021
//
// Post structs are what the database uses to link together posts in so that they can be manipulated in a reasonable way.


#include "PostNode.h"
#include "Post.h"
#include <stdlib.h>
#include <stdio.h>


POST_NODE* createPostNode(POST* post)
{
	POST_NODE* newPostNode = (POST_NODE*)malloc(sizeof(POST_NODE));
	
	newPostNode->NodeData = post;
	newPostNode->next = NULL;

	return newPostNode;
}

void setPostNodeNextPostNode(POST_NODE* sourceNode, POST_NODE* nextNode)
{
	sourceNode->next = nextNode;
}

POST_NODE* getPostNodeNextPostNode(POST_NODE* postnode)
{
	return postnode->next;
}

void disposePostNode(POST_NODE* postnode)
{
	disposePost(postnode->NodeData);
	free(postnode);
}