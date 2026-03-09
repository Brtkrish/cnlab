#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int isfaulty() {
    int d = rand() % 4;
    return (d > 2);
}

int main() {
    srand(time(0));
    int c_sock;

    c_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (c_sock < 0) { printf("Socket creation failed\n"); return 1; }

    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(7004);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(c_sock, (struct sockaddr *)&client, sizeof(client)) == -1) {
        printf("Connection failed\n");
        return 1;
    }

    printf("\nClient - with individual acknowledgement scheme\n\n");

    char msg1[50] = "Acknowledgement of: ";
    char msg3[50] = "Negative Acknowledgement: ";
    char msg2[50];
    char buff[100];
    int count = -1, flag = 1;

    while (count < 8) {
        memset(buff, 0, sizeof(buff));
        memset(msg2, 0, sizeof(msg2));

        if (count == 7 && flag == 1) {
            printf("here\n");
            flag = 0;
            recv(c_sock, buff, sizeof(buff), 0);
            continue;
        }

        int n = recv(c_sock, buff, sizeof(buff), 0);
        if (n < 0) { printf("Error in receiving\n"); break; }

        char i = buff[strlen(buff) - 1];
        printf("Message received from server: %s\n", buff);

        int isfault = isfaulty();
        printf("Corruption status: %d\n", isfault);
        printf("Acknowledgement sent for message.\n");

        if (isfault)
            strcpy(msg2, msg3);
        else {
            strcpy(msg2, msg1);
            count++;
        }

        // ✅ safer way to append character
        int len = strlen(msg2);
        msg2[len] = i;
        msg2[len + 1] = '\0';

        send(c_sock, msg2, sizeof(msg2), 0);
    }

    close(c_sock);
    return 0;
}