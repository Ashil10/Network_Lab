// Client side implementation of UDP client-server model 
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <time.h>
#define PORT 8081
#define MAXLINE 1024
// Driver 
 
int main() {
int sockfd;
char buffer[MAXLINE];
time_t t;
time(&t);
char *hello = ctime(&t); //"Hello from client : Current Time is ";
// char *currtime = 
ctime(&t); 
struct sockaddr_in servaddr;
// printf("\n current time is : %s",ctime(&t));
// Creating socket file descriptor
if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
perror("socket creation failed"); 
exit(EXIT_FAILURE);
}
memset(&servaddr, 0, sizeof(servaddr));
// Filling server information 
servaddr.sin_family = AF_INET; 
servaddr.sin_port = htons(PORT); 
servaddr.sin_addr.s_addr = INADDR_ANY;
int n, len;
sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr)); 
printf("Hello TIME  sent to the Server.\n");
n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
buffer[n] = '\0';
printf("Server TIME : %s\n", buffer);
close(sockfd); 
return 0;
}
