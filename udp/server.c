#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<time.h>
#define port 1234
#define size 1024

int main(){
struct sockaddr_in addr,cliaddr;
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
int n=bind(sock,(struct sockaddr *)&addr,sizeof(addr));
if(n<0){
perror("Bind error!!!");
exit(1);
}
recvfrom(sock,buffer,1024,0,(struct sockaddr *)&cliaddr,&addr_size);
printf("Client time :%s\n",buffer);
bzero(buffer,1024);
sleep(10);
time_t t;
char *time=ctime(&t);
strcpy(buffer,time);
strcat(buffer,"server time :");
sendto(sock,buffer,1024,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
close(sock);
return 0;
}
