// Server side implementation of UDP client-server model 
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
 
int main() 
{
     int sockfd;
     char buffer[MAXLINE];
     time_t t;
     time(&t);
     char *hello = ctime(&t); // "Hello from server";
     // char *currtime  = ctime(&t);
     struct sockaddr_in servaddr, cliaddr;
     // Creating socket file descriptor
     if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
     { 
     perror("socket creation failed"); 
     exit(EXIT_FAILURE);
     }
     memset(&servaddr, 0, sizeof(servaddr));
     memset(&cliaddr, 0, sizeof(cliaddr));
     // Filling server information 
     servaddr.sin_family = AF_INET; // IPv4
     servaddr.sin_addr.s_addr = INADDR_ANY; 
     servaddr.sin_port = htons(PORT);
     // Bind the socket with the server address
     if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
     {
     perror("bind failed"); 
     exit(EXIT_FAILURE);
     }
     int len, n;
     len = sizeof(cliaddr); //len is value/result
     n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
     buffer[n] = '\0';
     printf("Client TIME : %s\n", buffer); 
     sendto(sockfd, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
     printf("Hello TIME  sent to the Client .\n");
     return 0;
}
