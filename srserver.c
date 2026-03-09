#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void resend(int frame, int sock)
{
    char buff[100];
    sprintf(buff,"Resending message: %d",frame);

    printf("%s\n",buff);

    send(sock,buff,sizeof(buff),0);
}

int main()
{
    int s_sock,c_sock;

    s_sock = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server,client;

    server.sin_family = AF_INET;
    server.sin_port = htons(7004);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s_sock,(struct sockaddr*)&server,sizeof(server));

    listen(s_sock,5);

    socklen_t len = sizeof(client);

    c_sock = accept(s_sock,(struct sockaddr*)&client,&len);

    printf("\nServer Up - Selective Repeat ARQ\n\n");

    int total=0;

    char buff[100];

    fd_set set;
    struct timeval timeout;

    while(total<9)
    {
        int start = total;

        for(int i=start;i<start+3;i++)
        {
            sprintf(buff,"server message: %d",i);

            printf("Message sent to client: %s\n",buff);

            send(c_sock,buff,sizeof(buff),0);
        }

        for(int i=start;i<start+3;i++)
        {
            int ack=0;

            while(!ack)
            {
                FD_ZERO(&set);
                FD_SET(c_sock,&set);

                timeout.tv_sec=2;
                timeout.tv_usec=0;

                int rv = select(c_sock+1,&set,NULL,NULL,&timeout);

                if(rv==0)
                {
                    printf("\nTimeout for message: %d\n",i);
                    resend(i,c_sock);
                }

                else
                {
                    memset(buff,0,sizeof(buff));

                    recv(c_sock,buff,sizeof(buff),0);

                    printf("\nMessage from Client: %s\n",buff);

                    if(buff[0]=='n')
                    {
                        int frame = buff[strlen(buff)-1]-'0';

                        printf("Corrupt message %d, resending\n",frame);

                        resend(frame,c_sock);
                    }

                    else
                    {
                        total++;
                        ack=1;
                    }
                }
            }
        }
    }

    close(c_sock);
    close(s_sock);

}