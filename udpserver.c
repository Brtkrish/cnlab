// UDP Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server;
    char servMsg[2000], cliMsg[2000];
    struct sockaddr_in servAddr, client_addr;
    socklen_t client_struct_length = sizeof(client_addr);   

    server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server < 0) {
        printf("Error while creating socket\n");
        exit(1);
    }
    printf("1. Socket created successfully\n");

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(2002);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        printf("Couldn't bind to the port\n");
        exit(1);
    }
    printf("2. Binding done\n");
    printf("3. Listening...\n");

    memset(cliMsg, 0, sizeof(cliMsg));   
    if (recvfrom(server, cliMsg, sizeof(cliMsg), 0,
        (struct sockaddr*)&client_addr, &client_struct_length) < 0) {
        printf("Couldn't receive\n");
        exit(1);
    }
    printf("4. Message from client: %s\n", cliMsg);

    memset(servMsg, 0, sizeof(servMsg));   
    strcpy(servMsg, cliMsg);
    if (sendto(server, servMsg, strlen(servMsg), 0,
        (struct sockaddr*)&client_addr, client_struct_length) < 0) {
        printf("Can't send\n");
        exit(1);
    }
    printf("5. Message sent back to client\n");

    close(server);
    printf("6. Socket closed\n");

    return 0; 
}