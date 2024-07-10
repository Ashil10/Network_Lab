#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

void send_file(FILE *fp, int sockfd)
{
    char data[1024];
    while(fgets(data,1024,fp) != NULL)
    {
        if(send(sockfd, data, sizeof(data), 0) == -1)
        {
            perror("Error in sendind data");
            exit(1);
        }
        printf("%s\n",data);
        bzero(data,1024);
    }
}

void main()
{
    int port=5599;

    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_size;
    char buffer[1024];
    FILE *fp;
    char *filename = "send.txt";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=port;
    server_addr.sin_addr.s_addr=INADDR_ANY;

    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("connected to server\n");
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        perror("Error in reading");
        exit(1);
    }
    send_file(fp,sockfd);
    printf("File data send successfully\n");

    close(sockfd);
}

