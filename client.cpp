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

  // After the client is connected send string message
  string message = "PUT name angelo";

  int bytesSent = send(
    clientSocket,
    message.c_str(),
    static_cast<int>(message.size()),
    0
  );

  if(bytesSent == SOCKET_ERROR) {
    cout << "Send failed: " << WSAGetLastError() << "\n";
  }

  char buffer[1024];

  int bytesReceived = recv(
    clientSocket,
    buffer,
    sizeof(buffer) - 1,
    0
  );

  if (bytesReceived > 0) {
    buffer[bytesReceived] = '\0';
    cout << "Server response: " << buffer << "\n";
  }

  shutdown(clientSocket, SD_SEND);
  closesocket(clientSocket);
  WSACleanup();
  return 0;
}