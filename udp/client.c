#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<time.h>

#define port 1234
#define size 1024

int main(){
struct sockaddr_in addr,servaddr;
socklen_t addr_size;

int sock=socket(AF_INET,SOCK_DGRAM,0);
if(sock<0){
	perror("Socket error!!!\n");
	exit(1);
}
printf("Socket created!!");
addr.sin_family=AF_INET;
addr.sin_port=htons(port);
addr.sin_addr.s_addr=INADDR_ANY;

char buffer[size];

time_t t;
char *time=ctime(&t);
printf("%s",time);
sleep(10);

bzero(buffer,1024);
strcpy(buffer,time);
sendto(sock,buffer,1024,0,(struct sockaddr *)&addr,sizeof(addr));
bzero(buffer,1024);
recvfrom(sock,buffer,1024,0,(struct sockaddr *)&addr,&addr_size);
printf("Server:%s",buffer);
close(sock);
return 0;
}
