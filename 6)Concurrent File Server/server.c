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

void sendFile(FILE *fp,int sockfd)
{
	int n;
	char buffer[SIZE];
	char msg[50]="\nServer Process ID: ";
	bzero(buffer,SIZE);
	while( fgets(buffer,SIZE,fp) != NULL)
	{
		if( send(sockfd,buffer,sizeof(buffer),0) == -1)
		{
			break;
		}
		bzero(buffer,SIZE);
	}
	bzero(buffer,SIZE);
	int pid = getpid();
	char *mypid = malloc(6);
	sprintf(mypid,"%d",pid);
	strcpy(buffer,msg);
	strcat(buffer,mypid);
	send(sockfd,buffer,sizeof(buffer),0);
	bzero(buffer,SIZE);
	bzero(mypid,6);
	fclose(fp);
}

int main()
{
	int sockfd, clientSocket, pid, cnt=0;
	struct sockaddr_in serverAddr, cliAddr;
	socklen_t addr_size;
	pid_t childpid;
	char fname[50],buffer[SIZE],msg[100]="Requested file not found.\n\nServer Process ID: ";
	FILE *fp;
	bzero(fname,sizeof(fname));
	bzero(buffer,sizeof(buffer));
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		printf("Error in connection.\n");
		exit(1);
	}
	printf("[+]Server socket created..\n");
	memset(&serverAddr,'\0',sizeof(serverAddr));
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(PORT);
	serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if( bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0 )
	{
		perror("Bind error");
		exit(1);
	}
	if( listen(sockfd,10)==0 )
	{
		printf("[+]Server listening..\n");
	}
	while(1)
	{
		clientSocket = accept(sockfd,(struct sockaddr*)&cliAddr, &addr_size);
		if(clientSocket < 0)
		{
			//perror("Accept error");
			break;
		}
		printf("\n[+]Connection accepted from %s:%d\n",inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port));
		printf("[+]Client(s) connected: %d\n",++cnt);
		if( (childpid=fork()) == 0 )
		{
			close(sockfd);
			if( recv(clientSocket, fname, 50, 0) < 0)
			{
				perror("Error in receiving data from client");
				break;
			}
			if( access(fname,F_OK) == 0)
			{
				printf("[+]Requested file found..\n");
				fp=fopen(fname,"r");
				sendFile(fp,clientSocket);
				bzero(fname,sizeof(fname));
				printf("[+]Requested file sent to client successfully..\n");
			}
			else
			{
				printf("[+]Requested file not found..\n");
				bzero(buffer,SIZE);
				int pid = getpid();
				char *mypid = malloc(6);
				sprintf(mypid,"%d",pid);
				strcpy(buffer,msg);
				strcat(buffer,mypid);
				send(clientSocket,buffer,sizeof(buffer),0);
				bzero(fname,sizeof(fname));
				bzero(buffer,SIZE);
				bzero(mypid,6);
			}
		}
		close(clientSocket);
	}
	close(clientSocket);
	return 0;
}


