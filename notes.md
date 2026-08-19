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

- WSA = Window Sockets API
- WSADATA is a structure, which contains fields that store information about the initialized Winsock implementation

What's WSAStartup(MAKEWORD(2, 2), &wsaData)?

- WSAStartup is the function that initializes the Winsock library and fills the WSADATA structure with information about the installed Winsock implementation.

Why do we need WSACleanup() at the end?

- WSAClean() tells Windows that your program is finished using the Winsock library, allowing the operating system to release any resources it allocated during WSAStartup(). It's good practice to pair every successful WSAStartup() with a corresponding WSACleanup()

# Day 2

- Bind the socket to an IP address and port so the OS knows where your esrver should receive connections

sockaddr serverAddress

- Creates an empty structure that will store the server's IP address, address type, and port

serverAddress.sin_family = AF_INET | Address Family - Internet

- Specifies that the server will use IPv4 addresses

serverAddress.sin_addr.s_addr = INADDR_ANY | Internet Address Any

- telling the server to listen to all available IPv4 network interfaces/IP addresses on your computer
- sin_addr is a structure inside serverAddress, and s_addr is the actual IPv4 address value stored inside that structure

serverAddress.sin_port = htons(8080) | Host TO Network Short

- converts 8080 to a 16 bit value in network byte order
- Short means a short integer, traditionally 16-bit (2-byte)

bind(serverSocket, reinterpret_cast<sockaddr\*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR

- bind() attaches serverSocket to the IP address and port 8080 stored in serverAddress, and == SOCKET_ERROR checks whether that attachment failed

What's reinterpret cast?

- is a C++ cast that tells the compiler to treat the same memory/address as a different type, without actually changing the underlying data

Why do we need reinterpret cast?

- Bind() expects a sockaddr*, but we have a sockaddr_in* so reinterpret_cast lets us pass our IPv4 address structure to bind() in the type it expects

# Day 3

MAIN.CPP

Why do we need an array named buffer with 1024 available elements?

- the 1024 byte buffer provides temporary memory where recv() stores incoming TCP data. The size is an arbitrary practical limit for each read, not a requirement imposed by TCP. If more than 1024 bytes arrive, the remaining data must be sent with an additional recv() call.

- it's called buffer because it temporarily holds data while it moves between the network and your program

Why do we do sizeof(buffer) - 1?

- Leaves on byte available for the null terminator '\0', allowing the received data to be safely treated as a C-style string.

What's the 0 at the end in recv()?

- the final 0 means no special receive flags are enabled, so recv() uses its normal behavior.

What does recv return?

- recv() returns the number of bytes received, 0 if the client closed the connection normally, or SOCKET_ERROR (-1) if an error occurred. If the client sends "Hello", then recv() stores those 5 character in the buffer and returns 5.

if(bytesReceived > 0) {
buffer[bytesReceived] = '\0';
cout << "Client sent: " << buffer << '\n';
}

- buffer[bytesReceived] sets the index at bytesReceived to \0, the null terminator. When the buffer is treated as a string, functions like cout read from the beginning and stop when they reach that \0.

CLIENT.CPP

What does send() return?

- send() returns the number of bytes successfully accepted for sending, or SOCKET_ERROR -1 if an error occurs; it may retunr fewer bytes than requested.

EXAMPLE: If you send "Hello" (5 bytes), send() will usually return 5, meaning all 5 bytes were accepted for sending. If you request that 5 bytes be sent but send() returns 3, only "Hel" was accepted, so you must called send() again with the remaining "lo".

What arguments does send() accept?

- clientSocket: the connected socket
- message.c_str(): the data to send
- message.size(): the number of bytes to send
- 0: no spceial sending options (flags)

What's message.c_str()?

- returns a pointer to the characters inside the C++ std::string, allowing send() to read and transmit them. It returns the string's characters in the null terminated format traditionally used by C.

message.c_str()
│
▼
┌───┬───┬───┬───┬───┬──────┐
│ H │ e │ l │ l │ o │ '\0' │
└───┴───┴───┴───┴───┴──────┘
▲
pointer

What's static_cast?

- static_cast explicity converts a value from one compatible type to another at compile time, such as converting message.size() from size_t to the int expected by send()

Why do we need shutdown, closesocket to send the data?

- It was failing because WSACleanup() ran immediately after send() while the socket was still open, potentially discarding data that TCP had not transmitted yet. shutdown() and closesocket() give the connection an orderly ending before Winsock is cleaned up.

What's the difference between shutdown, closesocket and WSACleanup?

- shutdown: changes the TCP connection state. It tells Winsock that your program will send no more bytes, so TCP transmits any queued bytes and then sends a FIN. The socket handle remains valid, and you can still receive data.

- closesocket: releases that specific socket handle and its associated resources, such as send/receiuve buffers and TCP connection state. With normal socket settings, windows continues handling an orderly TCP close

- wsacleanup: matches a previous WSAStartup() call and decreases Winsock's per process reference count. When the count reaches zero, Winsock terminates the program's use of the library and release its remaining resources

What's SD_SEND in shutdown(clientSocket, SD_SEND)?

- SD_SEND tells shutdown() to disable only the sending side of the socket; queued data is transmitted, TCP signals that no more data is coming, and the socket may still receive data.

Other options are: SD_RECEIVE: stop receiving, SD_BOTH: stop both sending and receiving

# Day 5

#include <sstream> gives you string stream tools like stringstream, which lets you and write to strings

string request(buffer); creates a C++ std::string named request and initializes it with thes text stored in the C-Style character array buffer
