#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <winsock2.h> //on unix systems, use #include <sys/socket.h>, windows :((
#include <ws2tcpip.h> //read comment above^

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct {
    SOCKET socket;
    struct sockaddr_in address;
};

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    SOCKET server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};


    //Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    //Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed with error: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    
    //Bind the socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    //Listen for incoming connections
    if (listen(server_fd, 5) == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d\n", PORT);

    //accept connections
    if((client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) == INVALID_SOCKET) {
        printf("Accept failed with error: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    
    //Receive data from client
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    // buffer[BUFFER_SIZE - 1] = '\0';
    printf("Received: %s\n", buffer);

    const char* response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 21\r\n"
        "\r\n"
        "<h1>Hello World!</h1>";

    // Send data to client
    send(client_socket, response, strlen(response), 0);

    // Close the socket
    closesocket(client_socket);
    closesocket(server_fd);

    WSACleanup();

    return 0;
}