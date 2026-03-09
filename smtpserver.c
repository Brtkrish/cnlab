#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    struct sockaddr_in server, client;
    char str[50], msg[20];

    if (argc != 2) {
        printf("Input format not correct\n");
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Error in socket()\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_port = htons(atoi(argv[1]));

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Error in bind()\n");
        return 1;
    }

    socklen_t client_len = sizeof(client);
    printf("Server waiting...\n");
    sleep(3);

    if (recvfrom(sockfd, str, 100, 0, (struct sockaddr *)&client, &client_len) < 0)
        printf("Error in recvfrom()\n");
    printf("Got message from client: %s\n", str);

    printf("Sending greeting message to client\n");
    strcpy(str, "220 127.0.0.1");
    sleep(10);
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
        printf("Error in send\n");

    sleep(3);
    if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr *)&client, &client_len) < 0)
        printf("Error in recv\n");
    if (strncmp(str, "HELO", 4) != 0)
        printf("'HELO' expected from client\n");
    printf("%s\n", str);
    printf("Sending response...\n");
    strcpy(str, "250 ok");
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
        printf("Error in send\n");

    sleep(3);
    if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr *)&client, &client_len) < 0)
        printf("Error in recv\n");
    if (strncmp(str, "MAIL FROM", 9) != 0)
        printf("MAIL FROM expected from client\n");
    printf("%s\n", str);
    printf("Sending response...\n");
    strcpy(str, "250 ok");
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
        printf("Error in send\n");

    sleep(3);
    if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr *)&client, &client_len) < 0)
        printf("Error in recv\n");
    if (strncmp(str, "RCPT TO", 7) != 0)
        printf("RCPT TO expected from client\n");
    printf("%s\n", str);
    printf("Sending response...\n");
    strcpy(str, "250 ok");
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
        printf("Error in send\n");

    sleep(3);
    if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr *)&client, &client_len) < 0)
        printf("Error in recv\n");
    if (strncmp(str, "DATA", 4) != 0)
        printf("DATA expected from client\n");
    printf("%s\n", str);
    printf("Sending response...\n");
    strcpy(str, "354 Go ahead");
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
        printf("Error in send\n");

    if (recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&client, &client_len) < 0)
        printf("Error in recv\n");
    printf("Mail body received\n");
    printf("%s\n", msg);

    if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr *)&client, &client_len) < 0)
        printf("Error in recv\n");
    if (strncmp(str, "QUIT", 4) != 0)
        printf("QUIT expected from client\n");
    printf("Sending quit...\n");
    strcpy(str, "221 OK");
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
        printf("Error in send\n");

    close(sockfd);
    return 0;
}



How to Run:
Terminal 1 (Server):
gcc smtp_server.c -o smtp_server
./smtp_server 5000