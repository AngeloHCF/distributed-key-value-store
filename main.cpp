#include <iostream>
#include <WinSock2.h> // Socket
#include <WS2tcpip.h> // TCP

using namespace std;

// Link against the Winsock library
#pragma comment(lib, "Ws2_32.lib")


int main() {
  
  // Initialize Winsock before creating any sockets
  WSADATA wsaData;
  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // Returns 0 on success
    cout << "WSAStartup Failed\n";
    return 1;
  }

  // Create a TCP socket
  SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(serverSocket == INVALID_SOCKET) {
    cout << "Failed to create socket\n";
    WSACleanup();
    return 1;
  }

  cout << "TCP Socket created successfully!\n";

  // Bind socket to my computer
  sockaddr_in serverAddress{};

  serverAddress.sin_family = AF_INET;

  serverAddress.sin_addr.s_addr = INADDR_ANY;

  serverAddress.sin_port = htons(8080);

  if(bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
    cout << "Bind failed\n";
    closesocket(serverSocket);
    WSACleanup();
    return 1;
  }

  cout << "Server bound to port 8080\n";


  // Program exits
  WSACleanup();
  return 0;
  
}