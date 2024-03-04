#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<arpa/inet.h>
void accept_new_connection(int server_socket, fd_set *all_sockets, int *num_sockets)
{
	    struct sockaddr_in client_addr;
	    int client_socket, client_address_size;
	    client_socket=accept(server_socket, (struct sockaddr*) &client_addr, &client_address_size);
	    if(client_socket==-1)
	    {
		perror("Accept Error");
		exit(1);
	    }
	    FD_SET(client_socket, all_sockets);
	    *num_sockets = *num_sockets > client_socket? *num_sockets:client_socket;

	    printf("Client %d (port %d) connected.\n", client_socket-3, ntohs(client_addr.sin_port));
	    fflush(stdout);	    
}

void receive_and_broadcast(int client_socket, int server_socket, fd_set *all_sockets, int num_sockets)
{
	    char recieve_buffer[235];
	    char send_msg[256];
	    int bufSize;
	    int send_status, recieve_status;
	    recieve_status = recv(client_socket, &recieve_buffer, 235, 0);
	    if(recieve_status<=0)
	    {
		if(recieve_status==0)
		{	
		    printf("Client %d has disconnected.\n", client_socket-3);
		    fflush(stdout);
		}
		else 
		    perror("Receive Error.");
		
		close(client_socket);
		FD_CLR(client_socket, all_sockets);
	    }
	    else 
	    {
		recieve_buffer[recieve_status] = 0;
		sprintf(send_msg, "Client %d: %s", client_socket-3, recieve_buffer);
		for(int i=0; i<=num_sockets; i++)
		{
		    if(FD_ISSET(i, all_sockets))
		    {
		        if(i!=client_socket && i!=server_socket)
		        {
		            // printf("%s", send_msg);
		            if(send(i, &send_msg, sizeof(send_msg), 0)==-1)
		                perror("Send Error");
		            
		        }
		    }
		}
	    }
}


int main()
{
    int server_socket;
    // connect_request
    struct sockaddr_in server_addr;
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0))==-1) 
    {
        perror("Socket Error");
        exit(1);
    }
    printf("Server Socket: %d\n", server_socket);
    fflush(stdout);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4951);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int opt_value = 1;

    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(int))==-1)
    {
        perror("Socket option Error");
        exit(1);
    }

    if(bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr))==-1)
    {
        perror("Binding Error");
        exit(1);
    }

    if(listen(server_socket, 10)==-1)
    {
        perror("Error while listening");
        exit(1);
    }

    printf("TCP Server running on port 4950. \nWaiting clients to join::\n");
    fflush(stdout);

    // server, clien1, client2, ..
// 0 - stdin
// 1 - stdout
// 2 - stderr

    // fd: file descriptor
    // all_sockets: it is a set of all file descriptors (all sockets), including the servers and all the clients connected to the server.
    // selected_sockets: this is the output of select function, and it would store the set of file descriptors (sockets) which have sent message to the server.
    fd_set all_sockets, selected_sockets;
    int num_sockets;
    FD_ZERO(&all_sockets);
    FD_SET(server_socket, &all_sockets);
    // FD_SET(0, &all_sockets);

    num_sockets = server_socket;

    while(1)
    {
        selected_sockets = all_sockets;
        if(select(num_sockets+1, &selected_sockets, NULL, NULL, NULL)==-1)
        {
            perror("Select Error");
            exit(1);
        }

        for (int i=0; i<num_sockets+1; i++)
        {
            if (FD_ISSET(i, &selected_sockets))
            {
                if(i == server_socket)
                    accept_new_connection(server_socket, &all_sockets, &num_sockets);
                
                else
                    receive_and_broadcast(i, server_socket, &all_sockets, num_sockets);
                
            }
        }
    }
    close(server_socket);
    return 0;
}
