#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    FILE *fp;
    struct sockaddr_in servaddr;
    int sd, port, s;
    char name[1000], fname[1000], rcvg[1000];

    printf("Enter port number: ");
    scanf("%d", &port);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        printf("Socket creation error\n");
        exit(0);
    }
    printf("Socket is created...\n");

    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (connect(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("Connection error\n");
        exit(0);
    }
    printf("Connected...\n");

    printf("Enter the existing file name: ");
    scanf("%s", name);
    printf("Enter the new file name: ");
    scanf("%s", fname);

    fp = fopen(fname, "w");
    if (fp == NULL) {
        printf("Could not create file\n");
        exit(0);
    }

    send(sd, name, strlen(name), 0);

    while (1) {
        s = recv(sd, rcvg, sizeof(rcvg) - 1, 0);
        if (s <= 0) {
            printf("Error: File not received\n");
            break;
        }

        rcvg[s] = '\0';

        if (strcmp(rcvg, "error") == 0) {
            printf("File is not available\n");
            break;
        } else if (strcmp(rcvg, "completed") == 0) {
            printf("File is transferred...\n");
            fclose(fp);
            close(sd);
            break;
        } else {
            fwrite(rcvg, 1, s, fp);
        }
    }

    return 0;
}