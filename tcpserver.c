#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server, newSock;
    char buffer[1024];
    struct sockaddr_in servAddr;
    struct sockaddr_storage store;
    socklen_t addrSize;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("Socket creation failed");
        return 1;
    }
    printf("1. Socket created successfully.\n");

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6265);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        return 1;
    }
    printf("2. Bind successful.\n");

    if (listen(server, 5) == 0)
        printf("3. Listening on port 6265...\n");
    else {
        perror("Listen failed");
        return 1;
    }

    addrSize = sizeof(store);
    newSock = accept(server, (struct sockaddr*)&store, &addrSize);
    if (newSock < 0) {
        perror("Accept failed");
        return 1;
    }
    printf("4. Client connected.\n");

    memset(buffer, 0, sizeof(buffer));
    if (recv(newSock, buffer, sizeof(buffer), 0) < 0) {
        perror("Receive failed");
        return 1;
    }
    printf("5. Data received from client: %s", buffer);

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "Hi! This is Server. Message received.\n");
    printf("6. Sending data to client...\n");
    if (send(newSock, buffer, strlen(buffer), 0) < 0) {
        perror("Send failed");
        return 1;
    }
    printf("7. Data sent successfully.\n");

    close(newSock);
    close(server);
    printf("8. Connection closed.\n");

    return 0;
}