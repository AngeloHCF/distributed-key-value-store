#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

int main() {
  WSAData wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);

  // 1. Create TCP Socket
  SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // 2. Describe the server we want to connect to
  sockaddr_in serverAddress{};

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080);

  // 127.0.0.1 = this computer
  inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

  // 3. Connect to the server
  if(connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
    cout << "Connection failed\n";
    closesocket(clientSocket);
    WSACleanup();
    return 1;
  }

  cout << "Connected to server\n";
  WSACleanup();
  return 0;
}