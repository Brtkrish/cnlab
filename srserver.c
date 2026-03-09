#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

void rsendd(int ch, int c_sock) {
    char buff2[60];
    memset(buff2, 0, sizeof(buff2));
    strcpy(buff2, "Retransmission message: ");
    int len = strlen(buff2);
    buff2[len] = (ch) + '0';
    buff2[len + 1] = '\0';
    printf("Resending Message to client: %s\n", buff2);
    send(c_sock, buff2, sizeof(buff2), 0);
    usleep(1000);
}

int main() {
    int s_sock, c_sock;

    s_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (s_sock < 0) { printf("Socket creation failed\n"); return 1; }

    struct sockaddr_in server, other;
    memset(&server, 0, sizeof(server));
    memset(&other, 0, sizeof(other));
    server.sin_family = AF_INET;
    server.sin_port = htons(7004);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s_sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        printf("Binding failed\n"); return 1;
    }

    printf("\nSelective Repeat - Server\n\n");

    if (listen(s_sock, 10) < 0) { printf("Listen failed\n"); return 1; }

    socklen_t add = sizeof(other);
    c_sock = accept(s_sock, (struct sockaddr *)&other, &add);
    if (c_sock < 0) { printf("Accept failed\n"); return 1; }

    char buff[50];
    fd_set set1;
    struct timeval timeout1;
    int rv1;
    int tot = 0;

    while (tot < 9) {
        int toti = tot;

        // Send window of 3 frames
        for (int j = toti; j < (toti + 3); j++) {
            char buff2[60];
            memset(buff2, 0, sizeof(buff2));
            strcpy(buff2, "Server message: ");
            int len = strlen(buff2);
            buff2[len] = (j) + '0';
            buff2[len + 1] = '\0';
            printf("Message sent to client: %s\n", buff2);
            send(c_sock, buff2, sizeof(buff2), 0);
            usleep(1000);
        }

        // Wait for ACKs
        for (int k = toti; k < (toti + 3); k++) {
            int ack_received = 0;
            while (!ack_received) {                    // ✅ loop instead of goto
                FD_ZERO(&set1);
                FD_SET(c_sock, &set1);
                timeout1.tv_sec = 2;
                timeout1.tv_usec = 0;

                rv1 = select(c_sock + 1, &set1, NULL, NULL, &timeout1);
                if (rv1 == -1) {
                    perror("select error");
                    break;
                } else if (rv1 == 0) {
                    printf("Timeout for message: %d\n", k);
                    rsendd(k, c_sock);
                } else {
                    memset(buff, 0, sizeof(buff));
                    recv(c_sock, buff, sizeof(buff), 0);
                    printf("Message from Client: %s\n", buff);

                    if (buff[0] == 'n') {
                        printf("Corrupted message acknowledgement (msg %d)\n",
                            buff[strlen(buff) - 1] - '0');
                        rsendd((buff[strlen(buff) - 1] - '0'), c_sock);
                    } else {
                        tot++;
                        ack_received = 1;  // ✅ exit inner loop
                    }
                }
            }
        }
    }

    close(c_sock);
    close(s_sock);
    return 0;
}