#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

int main() {

    int server, newSock;
    struct sockaddr_in servAddr, store;
    socklen_t addrSize = sizeof(store);

    char buffer[1024];
    int expectedFrame = 0;

    srand(time(0));

    server = socket(PF_INET, SOCK_STREAM, 0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(5600);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(servAddr.sin_zero, '\0', sizeof(servAddr.sin_zero));

    bind(server, (struct sockaddr*)&servAddr, sizeof(servAddr));
    listen(server, 5);

    newSock = accept(server, (struct sockaddr*)&store, &addrSize);

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        recv(newSock, buffer, sizeof(buffer),0);

        int frame;
        char data[100];

        sscanf(buffer,"%d %[^\n]",&frame,data);

        printf("\n[SERVER] Frame %d Received: %s\n",frame,data);

        int random = rand()%100;

        if(random>80)
        {
            printf("[SERVER] ACK LOST (Random=%d)\n",random);
            continue;
        }

        if(frame==expectedFrame)
        {
            expectedFrame++;
            sprintf(buffer,"%d",expectedFrame);
            send(newSock,buffer,strlen(buffer),0);

            printf("[SERVER] ACK %d Sent (Random=%d)\n",expectedFrame,random);
        }
        else
        {
            sprintf(buffer,"%d",expectedFrame);
            send(newSock,buffer,strlen(buffer),0);

            printf("[SERVER] Duplicate Frame → ACK %d Resent\n",expectedFrame);
        }
    }

    close(newSock);
    close(server);
}