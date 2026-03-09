#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int client;
    char buffer[1024];
    struct sockaddr_in servAddr;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        perror("Socket creation failed");
        return 1;
    }
    printf("1. Socket created successfully.\n");

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6265);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Connection failed");
        return 1;
    }
    printf("2. Connected to server.\n");

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "Hi! This is Client. Are you there?\n");
    printf("3. Sending data to server...\n");
    if (send(client, buffer, strlen(buffer), 0) < 0) {
        perror("Send failed");
        return 1;
    }
    printf("4. Data sent successfully.\n");

    memset(buffer, 0, sizeof(buffer));
    if (recv(client, buffer, sizeof(buffer), 0) < 0) {
        perror("Receive failed");
        return 1;
    }
    printf("5. Data received from server: %s", buffer);

    close(client);
    printf("6. Connection closed.\n");

    return 0;
}