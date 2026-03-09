#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1000

int main() {
    int server_fd, new_socket, valread, port;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *error_msg = "error";
    char *completed_msg = "completed";
    FILE *fp;

    printf("Enter a port number: ");
    scanf("%d", &port);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", port);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("accept failed");
            continue;
        }
        printf("Connection established...\n");

        memset(buffer, 0, BUFFER_SIZE);
        valread = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (valread < 0) {
            send(new_socket, error_msg, strlen(error_msg), 0);
            close(new_socket);
            continue;
        }

        buffer[valread] = '\0';
        printf("Client requests file: %s\n", buffer);

        fp = fopen(buffer, "rb");
        if (fp == NULL) {
            send(new_socket, error_msg, strlen(error_msg), 0);
        } else {
            char file_buffer[BUFFER_SIZE];
            int bytes_read;
            while ((bytes_read = fread(file_buffer, 1, BUFFER_SIZE, fp)) > 0) {
                send(new_socket, file_buffer, bytes_read, 0);
            }
            fclose(fp);
            send(new_socket, completed_msg, strlen(completed_msg), 0);
        }

        close(new_socket);
        printf("Session completed. Listening for new connection...\n");
    }

    return 0;
}