#include <iostream>
#include <WinSock2.h> // Socket
#include <WS2tcpip.h> // TCP

// Link against the Winsock library
#pragma comment(lib, "Ws2_32.lib")

// Initialize Winsock before creating any sockets