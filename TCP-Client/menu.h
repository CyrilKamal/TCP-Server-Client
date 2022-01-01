// Cyril Kamal - Dec 2021
//
// Header file for menu.c


#pragma once

#define MAX_USER_INPUT	50
#define MAX_BUFFER		1000

#include "NetworkingFunctions.h"
#include <stdbool.h>

// PROTOTYPES
void printMainMenuOptions();
void executeUserOptionChoice(SOCKET);
void GetAllPosts(SOCKET);
void GetSpecificPost(SOCKET);
void UpdatePost(SOCKET);
void CreatePost(SOCKET);
void DeletePost(SOCKET);
bool isInputAnInteger(char*, int);