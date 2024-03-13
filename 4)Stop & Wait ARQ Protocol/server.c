#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5000
#define MAXSZ 100

typedef enum {DATA, ACK} FRAMEKIND;

struct timeval timeout;

struct Frame
{
	FRAMEKIND type;
	unsigned int len;
	int seq;
	char *msg;
};

struct Frame* makeframes()
{
	int i, seqno=1;
	struct Frame* fr=(struct Frame*)malloc(30*sizeof(struct Frame));
	for(i=0;i<30;i++)
	{
		fr[i].type=DATA;
		fr[i].seq=(seqno);
		fr[i].msg="$Mbdv,gl;hpoihewadbjlszncx.mvc,bg;/nh''wa?>SF<xdcfvgl;kjhgfdsasedrfg$";
		fr[i].len=strlen(fr->msg);
		seqno=fr[i].seq+1;
		
	}
	return fr;
}

int main()
{
	int sockfd,newsockfd,num;
	struct sockaddr_in serverAddress,clientAddress;
	struct Frame *frm;
	struct Frame f;
	int n,clientAdrLen,k;
	char msg[MAXSZ],msg1[MAXSZ];
	pid_t pid;
	timeout.tv_sec=2;
	timeout.tv_usec=0;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddress.sin_port=htons(PORT);
	bind(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
	listen(sockfd,5);
	int count=0;
	while(count<30)
	{
		printf("\nWaiting for connection..\n");
		clientAdrLen=sizeof(clientAddress);
		newsockfd=accept(sockfd,(struct sockaddr *)&clientAddress,&clientAdrLen);
		printf("Connected to client on %s at port %d\n",inet_ntoa(clientAddress.sin_addr),ntohs(clientAddress.sin_port));
		frm=makeframes();
		printf("\nHow many frames to send?\n");
		scanf("%d",&num);
		int expAck;
		while(count<num)
		{
			if(setsockopt(newsockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout))<0)
				perror("setsockopt(SO_RCVTIMEO) failed");
			f=frm[count];
			sprintf(msg,"%d%d%s",f.seq,f.len,f.msg);
			if(f.len>0)
			{
				k=send(newsockfd,msg,f.len,0);
				if(k>0)
				{
					printf("\nSent Frame %d\n",f.seq);
					expAck=((f.seq+1));
					n=recv(newsockfd,msg1,MAXSZ,0);
					if(n<=0) printf("No Ack Received\n");
					if(n>0)
					{
						printf("Received Ack %d\n",(msg1[0]-'0'));
						if(expAck==(msg1[0]-'0')) count++;
					}
				}
			}
		}
		close(newsockfd);
		printf("\nTransfer successful!\n");
		if(count==num) break;
	}
	return 0;
}

