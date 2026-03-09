#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2525
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd, 5);
    printf("SMTP Server running on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);
    send(client_fd, "220 Simple SMTP Server Ready\r\n", 30, 0);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes <= 0) break;

        printf("C: %s", buffer);

        if (strncmp(buffer, "HELO", 4) == 0) {
            send(client_fd, "250 Hello\r\n", 11, 0);
        } else if (strncmp(buffer, "MAIL FROM", 9) == 0) {
            send(client_fd, "250 OK\r\n", 8, 0);
        } else if (strncmp(buffer, "RCPT TO", 7) == 0) {
            send(client_fd, "250 OK\r\n", 8, 0);
        } else if (strncmp(buffer, "DATA", 4) == 0) {
            send(client_fd, "354 End data with <CR><LF>.<CR><LF>\r\n", 38, 0);
        } else if (strstr(buffer, "\r\n.\r\n") != NULL || strcmp(buffer, ".\r\n") == 0) {
            send(client_fd, "250 Message accepted\r\n", 23, 0);
        } else if (strncmp(buffer, "QUIT", 4) == 0) {
            send(client_fd, "221 Bye\r\n", 9, 0);
            break;
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
