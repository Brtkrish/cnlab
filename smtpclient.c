#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2525
#define BUFFER_SIZE 1024

void send_and_receive(int sock, const char *msg) {
    char buffer[BUFFER_SIZE];
    send(sock, msg, strlen(msg), 0);
    printf("C: %s", msg);
    memset(buffer, 0, BUFFER_SIZE);
    recv(sock, buffer, BUFFER_SIZE - 1, 0);
    printf("S: %s", buffer);
}

int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket failed");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    memset(buffer, 0, BUFFER_SIZE);
    recv(sock, buffer, BUFFER_SIZE - 1, 0);
    printf("S: %s", buffer);

    send_and_receive(sock, "HELO localhost\r\n");
    send_and_receive(sock, "MAIL FROM:<alice@example.com>\r\n");
    send_and_receive(sock, "RCPT TO:<bob@example.com>\r\n");
    send_and_receive(sock, "DATA\r\n");

    char body[1024];
    printf("Enter mail body : \n");
    if (fgets(body, sizeof(body), stdin) != NULL) {
        send(sock, body, strlen(body), 0);
        send(sock, "\r\n.\r\n", 5, 0);
        printf("C: <message body sent>\n");
    }

    memset(buffer, 0, BUFFER_SIZE);
    recv(sock, buffer, BUFFER_SIZE - 1, 0);
    printf("S: %s", buffer);

    send_and_receive(sock, "QUIT\r\n");

    close(sock);
    return 0;
}
