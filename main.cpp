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


  // Program exits
  WSACleanup();
  return 0;
  
}