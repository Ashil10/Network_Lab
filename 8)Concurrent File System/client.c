
//author :- kiran S Baliga

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 4444
#define SIZE 1024

void writeFile(int sockfd)
{
	int n;
	FILE *fp;
	char *fname = "output.txt";
	char buffer[SIZE];
	fp=fopen(fname,"w");
	while(1)
	{
		n=recv(sockfd,buffer,SIZE,0);
		if(n<=0)
		{
			break;
		}
		fprintf(fp,"%s",buffer);
		printf("%s",buffer);
		bzero(buffer,SIZE);
	}
	printf("\n\nFILE TRANSFER COMPLETE!\n");
	fclose(fp);
	return;
}

int main()
{
	int clientSocket;
	struct sockaddr_in serverAddr;
	char buffer[SIZE];
	char fname[50];
	clientSocket = socket(AF_INET,SOCK_STREAM,0);
	if( clientSocket < 0)
	{
		perror("Socket error");
		exit(1);
	}
	printf("[+]Client socket created..\n");
	memset(&serverAddr,'\0',sizeof(serverAddr));
	bzero(buffer,SIZE);
	bzero(fname,50);
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(PORT);
	serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if( connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0 )
	{
		perror("Connect error");
		exit(1);
	}
	printf("[+]Connected to server..\n\n");
	printf("Input the name of the file you want to request to server:-\n");
	scanf("%[^\n]%*c",fname);
	send(clientSocket,fname,sizeof(fname),0);
	printf("\n");
	writeFile(clientSocket);
	close(clientSocket);
	return 0;

}
