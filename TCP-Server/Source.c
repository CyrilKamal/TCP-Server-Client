#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x6000

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include "NetworkingFunctions.h"
#include "multiplexSystem.h"
#include "Posts.h"

#define LISTENINGPORT   "8080"      //needs to be a string

int main(void)
{
    InitializeWindowsSockets();

    printf("Config the local addr...\n");
    struct addrinfo* bind_address = ConfigureLocalAddress(LISTENINGPORT, TCP);

    printf("Creating socket...\n");
    SOCKET socket_listen = CreateBindListeningSocket(bind_address);

    printf("Start listening...\n");
    StartListeningForConnections(socket_listen);

    POSTS* postList = createPostList();
    loadPostsFromDrive(postList);

    SOCKET socket_client = WaitForAndAcceptAndHandleMultiplexedConnections(socket_listen, postList);

    return 0;
}