//authors :- athul n kiran
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int c_sock;
    c_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(9009);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");
	
    if(connect(c_sock, (struct sockaddr*)&client, sizeof(client)) == -1)
    {
        printf("Connection failed");
        return 0;
    }
    
    printf("\nClient started!\t(Individual acknowledgement scheme)\n\n");
    char msg1[50]="Acknowledgement of ";
    char msg2[50];
    char buff[100];
    int flag=1,flg=1;
    for(int i=0;i<=9;i++)
    {
    	flg=1;
    	bzero(buff,sizeof(buff));
    	bzero(msg2,sizeof(msg2));
    	/*if(i==8&&flag==1)
    	{
    		printf("Loss of frame %d\n",i);//simulating loss
    		flag=0;
    		read(c_sock,buff,sizeof(buff));
    	}*/
    	int n=read(c_sock, buff, sizeof(buff));
    	if(buff[strlen(buff)-1]!=i+'0')
    	{
        	printf("Discarded frame received since expected frame is %d.\n\n",i);
        	i--;
        }
        else
        {
        	printf("Message received from server: %s\n",buff);
        	printf("Acknowledgement sent for message %c.\n\n",buff[strlen(buff)-1]);
        	strcpy(msg2,msg1);
        	msg2[strlen(msg2)]=i+'0';
        	write(c_sock,msg2, sizeof(msg2));
        }
     }
     close(c_sock);
     return 0;
}
