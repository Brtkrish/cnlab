#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

int main() {
    int server_sock, acpt_sock, i = 0;
    char msg[50] = "Frame: ";
    char write_buff[50], read_buff[100];
    struct sockaddr_in serv_addr, other_addr;
    socklen_t len;
    fd_set set;
    struct timeval timeout;
    int ret_val;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) { printf("Socket creation failed\n"); exit(1); }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7004);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        printf("Binding failed\n"); exit(1);
    }

    printf("\nSender of Go Back N where N = 8\n\n");
    listen(server_sock, 5);

    len = sizeof(other_addr);
    acpt_sock = accept(server_sock, (struct sockaddr *)&other_addr, &len);
    if (acpt_sock < 0) { printf("Accept failed\n"); exit(1); }

    while (i < 8) {
        memset(write_buff, 0, sizeof(write_buff));
        strcpy(write_buff, msg);
        write_buff[strlen(msg)] = i + '0';
        printf("To Receiver -> Frame %d\n", i);
        send(acpt_sock, write_buff, sizeof(write_buff), 0);
        sleep(1);

        i++;
        memset(write_buff, 0, sizeof(write_buff));
        strcpy(write_buff, msg);
        write_buff[strlen(msg)] = i + '0';
        printf("To Receiver -> Frame %d\n", i);
        send(acpt_sock, write_buff, sizeof(write_buff), 0);

        FD_ZERO(&set);
        FD_SET(acpt_sock, &set);
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        ret_val = select(acpt_sock + 1, &set, NULL, NULL, &timeout);
        if (ret_val == -1) {
            printf("Error in select()\n");
            break;
        } else if (ret_val == 0) {
            printf("\n--- Timer Timeout - Frame %d Lost ---\n", i - 1);
            printf("--- Restarting - Resending from Frame %d ---\n\n", i - 1);
            i = i - 1;
        } else {
            memset(read_buff, 0, sizeof(read_buff));
            recv(acpt_sock, read_buff, sizeof(read_buff), 0);

            if ((i == 6) || (i == 4) || (i == 1))
                printf("--- From Receiver <- %s -- Cumulative ACK ---\n", read_buff);
            else
                printf("--- From Receiver <- %s -- Individual ACK ---\n", read_buff);

            printf("-----------------------------------------------------\n");
            i++;
        }
    }

    close(acpt_sock);
    close(server_sock);
    return 0;
}