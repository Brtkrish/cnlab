#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    struct sockaddr_in server, client;
    char str[50] = "hi";
    char mail_f[50], mail_to[50], msg[20];
    char c;
    int t = 0;
    socklen_t l = sizeof(server);

    if (argc != 3) {
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
    server.sin_port = htons(atoi(argv[2]));

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_port = htons(atoi(argv[2]));

    printf("Sending hi to server\n");
    sleep(10);
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, sizeof(server)) < 0)
        printf("Error in sendto\n");

    if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, &l) < 0)
        printf("Error in recv\n");
    printf("Greeting msg is %s\n", str);
    if (strncmp(str, "220", 3) != 0)
        printf("Connection not established, code 220 expected\n");

    printf("Sending HELO\n");
    strcpy(str, "HELO 127.0.0.1");
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, sizeof(server)) < 0)
        printf("Error in sendto\n");

    sleep(3);
    if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, &l) < 0)
        printf("Error in recv\n");
    if (strncmp(str, "250", 3) != 0)
        printf("Ok not received from server\n");
    printf("Server sent: %s\n", str);

    printf("Enter FROM address: ");
    scanf("%s", mail_f);
    strcpy(str, "MAIL FROM");
    strcat(str, mail_f);
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, sizeof(server)) < 0)
        printf("Error in sendto\n");

    sleep(3);
    if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, &l) < 0)
        printf("Error in recv\n");
    if (strncmp(str, "250", 3) != 0)
        printf("Ok not received from server\n");
    printf("%s\n", str);

    printf("Enter TO address: ");
    scanf("%s", mail_to);
    strcpy(str, "RCPT TO");
    strcat(str, mail_to);
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, sizeof(server)) < 0)
        printf("Error in sendto\n");

    sleep(3);
    if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, &l) < 0)
        printf("Error in recv\n");
    if (strncmp(str, "250", 3) != 0)
        printf("Ok not received from server\n");
    printf("%s\n", str);

    printf("Sending DATA to server\n");
    strcpy(str, "DATA");
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, sizeof(server)) < 0)
        printf("Error in sendto\n");

    sleep(3);
    if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, &l) < 0)
        printf("Error in recv\n");
    if (strncmp(str, "354", 3) != 0)
        printf("Ok not received from server\n");
    printf("%s\n", str);

    printf("Enter mail body (end with $): ");
    while (1) {
        c = getchar();
        if (c == '$') {
            msg[t] = '\0';
            break;
        }
        if (c == '\0')
            continue;
        msg[t++] = c;
    }

    if (sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&server, sizeof(server)) < 0)
        printf("Error in sendto\n");

    sleep(3);
    printf("Sending QUIT to server\n");
    strcpy(str, "QUIT");
    if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, sizeof(server)) < 0)
        printf("Error in sendto\n");

    if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr *)&server, &l) < 0)
        printf("Error in recv\n");
    if (strncmp(str, "221", 3) != 0)
        printf("Ok not received from server\n");
    printf("Server sent GOODBYE. Closing connection\n");
    printf("Bye\n");

    close(sockfd);
    return 0;
}

HOW TO RUN 
gcc smtp_client.c -o smtp_client
./smtp_client 127.0.0.1 5000