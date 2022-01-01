#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x6000
#endif

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <time.h>

#define DISPLAYBUFFERSIZE	100
#define SENDBUFFERSIZE	1000
#define MAXLISTENERS		10

typedef enum proto { UDP, TCP } PROTOCOL;

//common
void InitializeWindowsSockets();
void CloseSocketConnection(SOCKET);
void ShutdownWindowsSockets();

//server only
struct addrinfo* ConfigureLocalAddress(char*, PROTOCOL);
SOCKET CreateBindListeningSocket(struct addrinfo*);
void StartListeningForConnections(SOCKET);
SOCKET WaitForAndAcceptAndHandleMultiplexedConnections(SOCKET);

//client only
struct addrinfo* ConfigureRemoteAddress(char*, char*, PROTOCOL);
SOCKET CreateAndConnectRemoteSocket(struct addrinfo*);




