#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5000
#define SERVER_IP "127.0.0.1"
#define MAXSZ 100

typedef enum {DATA, ACK} FRAMEKIND;

struct MSG
{
	char data[MAXSZ];
};

struct timeval timeout;

struct Frame
{
	FRAMEKIND type;
	unsigned int len;
	int seq;
	char *msg;
};

int main()
{
	int sockfd;
	struct sockaddr_in serverAddress;
	int n,e,i,j,p=0,ack,exp=1,m;
	char msg1[MAXSZ],msg2[MAXSZ],msg3[MAXSZ],q,w;
	char del='$';
	struct Frame *frame=(struct Frame*)malloc(30*sizeof(struct Frame));
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=inet_addr(SERVER_IP);
	serverAddress.sin_port=htons(PORT);
	connect(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
	printf("\nWaiting to receive..\n");
	int count=0;
	while(count<30)
	{
		n=recv(sockfd,msg2,MAXSZ,0);
		if(n>0)
		{
			if(exp==(msg2[0]-'0'))
			{
				for(i=0;i<strlen(msg2);i++)
				{
					q=msg2[i];
					if(strcmp(&q,&del)==0)	break;
				}
				for(j=i;j<strlen(msg2);j++)
				{
					msg3[p++]=msg2[j];
					w=msg2[j];
					if(strcmp(&w,&del)==0) break;
				}
				frame->seq=(msg2[0]-'0');
				frame->type=DATA;
				frame->msg=msg3;
				frame->len=strlen(msg3);
				printf("\nReceived Frame %d\n",frame->seq);
				ack=(frame->seq+1);
				sprintf(msg1,"%d",ack);
				m=strlen(msg1)+1;
				srand(time(NULL));
				int sl=rand()%4;
				sleep(sl+1);
				send(sockfd,msg1,m,0);
				printf("Ack %d sent\n",ack);
				exp=(exp+1);
				count++;
			}
			
		}
		if(n==0) {printf("\nReceiver Exit\n"); break;}
	}
	return 0;
}
