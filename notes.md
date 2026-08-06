--Building a Distributed Key Value Store System--

# Day 1

- Build a TCP server that supports PUT, GET, and DELETE requests using an in memory hash map

What is TCP server?

- Program that listens for incoming TCP connections from clients, accepts them, and exchanges reliable data over those connections
  What's a TCP connection?
  - TCP connection is a reliable communication channel between two established computers that guarantees data arrives in order, without duplication, and without loss (or retransmits lost data)
    <Analogy>
    Imagine you and a friend are having a phone call. Before either of you starts talking, you both answer the call and establish the connection, then you take turns speaking while knowing the other person can hear you. If a word is missed because of static, you ask them to repeat it, ensuring the conversation remains complete and in the correct order.
    <Analogy>

#include <WinSock2.h> Windows Sockets 2 includes the declarations for the core Windows Sockets API, such as socket(), bind(), listen(), connect(), send(), and recv().
#include <WS2tcpip.h> Window Sockets 2 TCP/IP includes additional TCP/IP-related declarations, such as IP address conversion functions (inet_pton, inet_ntop) and structures used for IPv4 and IPv6

Why do need IP conversion functions?

- To convert IP addresses like 192.168.1.10 into the binary format that the operating system and network hardware can actually use

What's the difference between IPv4 and IPv6?

- IPv4 uses 32 bit addresses (e.g., 192.168.1.1), while IPv6 uses 128 bit addresses (e.g., 2001:db8::1) providing a vastly larger number of unique IP addresses

Why do I need #include <WS2tcpip.h> if I have <WinSock2.h>?

- You don't need <WS2tcpip.h> to create a basic TCP server. <WS2tcpip.h> is only needed if you use additional TCP/IP features such as IPv6 or modern IP address conversion functions.

What's WSADATA?

- WSADATA is a structure, which contains fields that store information about the initialized Winsock implementation

What's WSAStartup(MAKEWORD(2, 2), &wsaData)?

- WSAStartup is the function that initializes the Winsock library and fills the WSADATA structure with information about the installed Winsock implementation.

Why do we need WSACleanup() at the end?

- WSAClean() tells Windows that your program is finished using the Winsock library, allowing the operating system to release any resources it allocated during WSAStartup(). It's good practice to pair every successful WSAStartup() with a corresponding WSACleanup()
