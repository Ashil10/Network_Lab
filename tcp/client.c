#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 4444
#define SIZE 1024

int main(){
	struct sockaddr_in addr;
	socklen_t addr_size;
	char buffer[SIZE];
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1)
	{
	perror("Socket Error!!");
	exit(1);
	}
	printf("Socket created!!\n");
	addr.sin_family=AF_INET;
	addr.sin_port=htons(PORT);
	addr.sin_addr.s_addr=INADDR_ANY;
	
	int cstatus=connect(sock,(struct sockaddr *)&addr,sizeof(addr));
	
	if(cstatus<0){
	perror("Connection Error !!");
	exit(1);
	}
	printf("Connected to server!!\n");
	while(1){
	bzero(buffer,1024);
	fgets(buffer,1024,stdin);
	send(sock,buffer,sizeof(buffer),0);
	}
	close(sock);
	return 0;
}
