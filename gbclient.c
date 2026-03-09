#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int cli_sock, i, flag = 1;
    char write_buff[50], read_buff[100];
    struct sockaddr_in client;

    cli_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_sock < 0) { printf("Socket creation failed\n"); exit(1); }

    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(7004);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(cli_sock, (struct sockaddr *)&client, sizeof(client)) == -1) {
        printf("Connection failed\n"); exit(1);
    }

    printf("\nReceiver of Go Back N where N = 8\n\n");

    for (i = 0; i < 8; i++) {
        memset(read_buff, 0, sizeof(read_buff));
        memset(write_buff, 0, sizeof(write_buff));

        if (i == 5 && flag == 1) {
            flag = 0;
            recv(cli_sock, read_buff, sizeof(read_buff), 0);
            printf("\n--- Packet from Sender %d Lost - Discarding ---\n", i);
            i--;
            continue;
        }

        recv(cli_sock, read_buff, sizeof(read_buff), 0);

        if (read_buff[strlen(read_buff) - 1] != i + '0') {
            printf("\n--- Frame %d Received Out of Order - Discarding ---\n", i + 1);
            i--;
        } else {
            printf("-> %s\n", read_buff);

            strcpy(write_buff, "ACK: ");
            write_buff[strlen(write_buff)] = i + '0';
            write_buff[strlen(write_buff)] = '\0';
            send(cli_sock, write_buff, sizeof(write_buff), 0);
            printf("ACK sent for frame %d\n", i);
        }
    }

    close(cli_sock);
    return 0;
}