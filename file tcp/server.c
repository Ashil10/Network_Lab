#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>

void write_file(int new_sock)
{
    int n;
    FILE *fp;
    char *filename = "output.txt";
    char buffer[1024];

    fp = fopen(filename, "w");
    // while(1)
    // {
        n = recv(new_sock, buffer, 1024, 0);
        printf("buffer: %s",buffer);
        if(n<0)
        {
            printf("inside loop\n");
            // break;
            return;
        }
        printf("data: %s\n",buffer);
        fprintf(fp, "%s", buffer);
        bzero(buffer,1024);
    // }
    fclose(fp);
    return;
}

void main()
{
    int port=5599;

    int sockfd,new_sock;
    struct sockaddr_in server_addr,new_addr;
    socklen_t addr_size;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=port;
    server_addr.sin_addr.s_addr=INADDR_ANY;

    int n;
    n=bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("Bind to port %d\n",port);

    listen(sockfd,10);
    printf("Listening...\n");

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

    write_file(new_sock);
    printf("Data written successfully\n");
    
}

