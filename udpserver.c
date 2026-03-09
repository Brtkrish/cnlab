#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int serversocket, port;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    char message[1024];

    serversocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serversocket < 0) {
        perror("Socket failed");
        exit(1);
    }

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    printf("Enter the port number: ");
    scanf("%d", &port);
    serveraddr.sin_port = htons(port);

    if (bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    len = sizeof(clientaddr);
    printf("\nWaiting for message from client...\n");

    memset(message, 0, sizeof(message));
    ssize_t n = recvfrom(serversocket, message, sizeof(message) - 1, 0, (struct sockaddr*)&clientaddr, &len);

    if (n > 0) {
        message[n] = '\0';
        printf("Message received from client: %s\n", message);

        const char *response = "HELLO CLIENT...";
        sendto(serversocket, response, strlen(response), 0, (struct sockaddr*)&clientaddr, len);
    }

    close(serversocket);
    return 0;
}
