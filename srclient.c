#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int isfaulty()
{
    int d = rand()%4;
    return (d>2);
}

int main()
{
    srand(time(0));

    int c_sock;

    c_sock = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(7004);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(c_sock,(struct sockaddr*)&server,sizeof(server));

    printf("\nClient with individual acknowledgement (Selective Repeat)\n\n");

    char buff[100];
    char msg[50];

    int count=-1;

    while(count<8)
    {
        memset(buff,0,sizeof(buff));

        recv(c_sock,buff,sizeof(buff),0);

        printf("Message received from server: %s\n",buff);

        int frame = buff[strlen(buff)-1]-'0';

        int fault = isfaulty();

        printf("Corruption status: %d\n",fault);

        if(fault)
        {
            sprintf(msg,"nack%d",frame);
            printf("Negative ACK sent for message %d\n\n",frame);
        }

        else
        {
            sprintf(msg,"ack%d",frame);
            printf("ACK sent for message %d\n\n",frame);
            count++;
        }

        send(c_sock,msg,sizeof(msg),0);
    }

    close(c_sock);
}