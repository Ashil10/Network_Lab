#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>

void send_msg(int client_socket)
{
    char message[256];
    fgets(message, 256, stdin);
    if(strcmp(message, "quit\n")==0)
    {
        printf("\nExiting...\n");
        fflush(stdout);
        exit(0);
    }
    else 
    {
        // printf("%s", message);
        send(client_socket, message, strlen(message), 0);
    }
}

void receive_msg(int client_socket)
{
    char message[256];
    int recieve_status = recv(client_socket, message, sizeof(message), 0);
    if(recieve_status<=0)
    {
        if(recieve_status==-1)
            perror("Receive Error");
        
        else
            perror("Server Offline");
        
        close(client_socket);
        exit(1);
    }
    else
    {
        printf("\n%s",message);
        fflush(stdout);
    }
}

int main()
{

    int client_socket;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket==-1)
    {
        perror("Client Socket Creation Error");
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4951);
    server_addr.sin_addr.s_addr = INADDR_ANY;
   
    int connection_status = connect(client_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if(connection_status==-1)
    {
        perror("Connection error");
        exit(1);
    }
    printf("Connected to server.\n");
    fflush(stdout);

    fd_set all_sockets, selected_sockets;
   
    FD_ZERO(&all_sockets);
    FD_SET(0, &all_sockets);
    FD_SET(client_socket, &all_sockets);

    while(1) 
    {
        selected_sockets = all_sockets;
        if(select(client_socket+1, &selected_sockets, NULL, NULL, NULL)==-1)
        {
            perror("Select");
            exit(1);
        }

        if(FD_ISSET(0, &selected_sockets))
            send_msg(client_socket);
    
        if(FD_ISSET(client_socket, &selected_sockets))
            receive_msg(client_socket);
        
    }

    close(client_socket);

    return 0;
}


