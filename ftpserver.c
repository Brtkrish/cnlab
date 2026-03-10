#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// This structure mimics a file system within the code
struct FileStore {
    char name[50];
    char content[500];
};

// Internal "Sample Files" stored in the server's memory
struct FileStore myFiles[3] = {
    {"hello.txt", "Hello! This is content from the internal 'hello.txt' file."},
    {"lab.txt", "Networking Lab Exam: FTP simulation using internal structures."},
    {"test.txt", "Test successful. Data retrieved from server memory."}
};

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char request[BUFFER_SIZE] = {0};

    // Receive the filename request from the client
    recv(client_socket, request, BUFFER_SIZE, 0);
    printf("Client requested: %s\n", request);

    int found = 0;
    // Loop through the internal array to find the "file"
    for(int i = 0; i < 3; i++) {
        if (strcmp(request, myFiles[i].name) == 0) {
            send(client_socket, myFiles[i].content, strlen(myFiles[i].content), 0);
            printf("File sent successfully.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        char *error = "Error: File not found in server memory.";
        send(client_socket, error, strlen(error), 0);
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 3);

    printf("FTP Server (Internal Storage) listening on port %d\n", PORT);

    while(1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        printf("Client connected\n");
        handle_client(client_socket);
    }
    return 0;
}
