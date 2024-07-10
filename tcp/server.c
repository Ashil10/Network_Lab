#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 4444
#define SIZE 1024

int main(){
	struct sockaddr_in addr,cli_addr;
	socklen_t addr_size;
	char buffer[SIZE];
	int server_sock=socket(AF_INET,SOCK_STREAM,0);
	if(server_sock==-1)
	{
	perror("Socket Error!!");
	exit(1);
	}
	printf("Socket created!!\n");
	addr.sin_family=AF_INET;
	addr.sin_port=htons(PORT);
	addr.sin_addr.s_addr=INADDR_ANY;
	
	int n=bind(server_sock,(struct sockaddr *)&addr,sizeof(addr));
	if(n<0){
	perror("bind error");
	exit(1);
	}
	printf("Bind to port!!\n");
	int lstatus=listen(server_sock,5);
	if(lstatus<0)
	{
	perror("listen error!!!\n");
	exit(1);
	}
	printf("Server listening!!\n");
	int client_sock=accept(server_sock,(struct sockaddr *)&cli_addr,&addr_size);
	if(client_sock<0){
	perror("accept error!!");
	}
	printf("Client connected!!");
	while(1){
	bzero(buffer,1024);
	recv(client_sock,buffer,sizeof(buffer),0);
	printf("Client:%s\n",buffer);
	}
	close(client_sock);
	
}
