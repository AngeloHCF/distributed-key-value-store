#include <iostream>
#include <WinSock2.h> // Socket
#include <WS2tcpip.h> // TCP
#include <sstream> // stringstream
#include <unordered_map>

using namespace std;

// Link against the Winsock library
#pragma comment(lib, "Ws2_32.lib")


int main() {
  // Store the key-value pair in an unordered_map
  unordered_map<string, string> store;
  
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

  cout << "Server is listening on port 8080...\n";

  // Make the socket listen for incoming client connections, you only need to put the server into listening mode once
  if(listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
    cout << "Listen failed\n";
    closesocket(serverSocket);
    WSACleanup();
    return 1;
  }

  // Make server stay alive and accept multiple clients
  while(true) {  

    // Accept so server can wait for a client to connect
    sockaddr_in clientAddress{};
    int clientSize = sizeof(clientAddress);
    
    SOCKET clientSocket = accept(
      serverSocket,
      reinterpret_cast<sockaddr*>(&clientAddress),
      &clientSize
    );
    
    if(clientSocket == INVALID_SOCKET) {
      cout << "Accept failed\n";
      closesocket(serverSocket);
      WSACleanup();
      return 1;
    }
    
    cout << "Client connected\n";
    
    // Receive data from the connected client using recv()
    char buffer[1024];
    
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    if(bytesReceived > 0) {
      buffer[bytesReceived] = '\0';
      cout << "Client sent: " << buffer << '\n';
    }
    
    // Key-value store protocol
    string request(buffer);
    
    stringstream ss(request);
    
    string command;
    string key;
    string value;
    
    ss >> command >> key >> value;
    
    // cout << "Command: " << command << "\n";
    // cout << "Key: " << key << "\n";
    // cout << "Value: " << value << "\n";
    
    string response;
    
    if(command == "PUT") {
      store[key] = value;
      response = "OK\n";
    } else if(command == "GET") {
      if(store.count(key))
        response = store[key];
      else
        response = "Key not found!\n";
    } else if (command == "DELETE") {
      if(store.count(key)) {
        store.erase(key);
        response = "Deleted\n";
      } else {
        response = "Key not found!\n";
      }
    }
    
    send(clientSocket, response.c_str(), static_cast<int>(response.size()), 0);
  }
    
    // Program exits
    WSACleanup();
  return 0;
  
}