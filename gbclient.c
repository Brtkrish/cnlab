#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
 
int main() 
{ 
    int c_sock; 
    struct sockaddr_in server; 
    c_sock = socket(AF_INET, SOCK_STREAM, 0); 
    server.sin_family = AF_INET; 
    server.sin_port = htons(9009); 
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    connect(c_sock, (struct sockaddr *)&server, sizeof(server)); 
    printf("\nClient using Individual Acknowledgement\n\n"); 
 
    char buff[50], ack[50]; 
    int expected = 0, loss_simulated = 0; 
 
    while (expected < 10) 
    { 
        read(c_sock, buff, sizeof(buff)); 
        int seq = buff[strlen(buff) - 1] - '0'; 
        if (seq != expected) 
        { 
            printf("Discarded out-of-order frame %d\n", seq); 
            continue; 
        } 
 
        if (seq == 8 && loss_simulated == 0) 
        {
            printf("Simulating loss of ACK for frame 8\n\n"); 
            loss_simulated = 1; 
            continue; 
        } 
 
        printf("Received: %s\n", buff); 
        sprintf(ack, "ACK %d", seq); 
        write(c_sock, ack, strlen(ack) + 1); 
        printf("Sent: %s\n\n", ack); 
 
        expected++; 
    } 
 
    close(c_sock); 
    return 0; 
} 