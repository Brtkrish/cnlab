#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int clientsocket, port;
    struct sockaddr_in serveraddr;
    socklen_t len;
    char message[1024];

    clientsocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientsocket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    printf("Enter the port number: ");
    scanf("%d", &port);
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(serveraddr);

    printf("\nSending message for server connection\n");
    const char *initial_msg = "HI I AM CLIENT...";
    sendto(clientsocket, initial_msg, strlen(initial_msg), 0, (struct sockaddr*)&serveraddr, len);

    printf("Receiving message from server.\n");
    memset(message, 0, sizeof(message));
    ssize_t n = recvfrom(clientsocket, message, sizeof(message) - 1, 0, (struct sockaddr*)&serveraddr, &len);

    if (n >= 0) {
        message[n] = '\0';
        printf("Message received: %s\n", message);
    }

    close(clientsocket);
    return 0;
}
