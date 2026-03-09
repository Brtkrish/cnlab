#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <sys/select.h> 
 
int main() 
{ 
int s_sock, c_sock; 
struct sockaddr_in server, client; 
socklen_t len = sizeof(client); 
s_sock = socket(AF_INET, SOCK_STREAM, 0); 
server.sin_family = AF_INET; 
server.sin_port = htons(9009); 
server.sin_addr.s_addr = INADDR_ANY; 
 
bind(s_sock, (struct sockaddr *)&server, sizeof(server)); 
listen(s_sock, 5); 
 
printf("\nServer UP\nGo-Back-N protocol (N = 3)\n\n"); 
c_sock = accept(s_sock, (struct sockaddr *)&client, &len); 
int base = 0, nextSeq = 0; 
char msg[50], ack[50]; 
fd_set readfds; 
struct timeval tv; 
 
while (base < 10) 
{ 
while (nextSeq < base + 3 && nextSeq < 10) 
{ 
sprintf(msg, "Frame %d", nextSeq); 
write(c_sock, msg, strlen(msg) + 1); 
printf("Sent: %s\n", msg); 
nextSeq++; 
} 
 
FD_ZERO(&readfds);
FD_SET(c_sock, &readfds); 
tv.tv_sec = 2; 
tv.tv_usec = 0; 
int rv = select(c_sock + 1, &readfds, NULL, NULL, &tv); 
if (rv == 0) 
{ 
printf("Timeout! Retransmitting from frame %d\n\n", base); 
nextSeq = base; 
} 
else 
{ 
read(c_sock, ack, sizeof(ack)); 
printf("Received ACK: %s\n\n", ack); 
base++; 
} 
} 
 
close(c_sock); 
close(s_sock); 
return 0; 
} 