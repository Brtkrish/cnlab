#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

int main()
{

    int client;
    struct sockaddr_in servAddr;

    char buffer[1024];
    char data[100];

    int frame = 0;

    srand(time(0));

    client = socket(PF_INET, SOCK_STREAM, 0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(5600);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client,(struct sockaddr*)&servAddr,sizeof(servAddr));

    while(1)
    {

        printf("\nEnter Data: ");
        fgets(data,sizeof(data),stdin);
        data[strcspn(data,"\n")] = 0;

        sprintf(buffer,"%d %s",frame,data);

        int random = rand()%100;

        send(client,buffer,strlen(buffer),0);

        printf("[CLIENT] Frame %d Sent (Random=%d)\n",frame,random);

        memset(buffer,0,sizeof(buffer));

        sleep(1);

        int n = recv(client,buffer,sizeof(buffer),MSG_DONTWAIT);

        if(n<=0)
        {
            printf("[CLIENT] Timeout / ACK Lost → Retransmitting...\n");
            continue;
        }

        int ack;
        sscanf(buffer,"%d",&ack);

        printf("[CLIENT] ACK %d Received\n",ack);

        frame++;
    }

    close(client);
}