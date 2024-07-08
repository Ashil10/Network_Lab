#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define SMTP_SERVER "smtp.example.com"
#define SMTP_PORT 587
#define BUFFER_SIZE 1024

typedef enum {DATA, AUTH_LOGIN, AUTH_USERNAME, AUTH_PASSWORD, SECURITY_QUESTION} SMTP_STATE;

int send_command(int sockfd, char* command, char* response);
int send_email(int sockfd, char* from, char* to, char* subject, char* message);

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    SMTP_STATE state = DATA;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address setup
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SMTP_PORT);
    if (inet_pton(AF_INET, SMTP_SERVER, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Receive server response
    if (recv(sockfd, buffer, BUFFER_SIZE, 0) < 0) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", buffer);

    // SMTP client states
    switch (state) {
        case DATA:
            // Send HELO command
            send_command(sockfd, "HELO localhost\r\n", buffer);

            // Send STARTTLS command if using TLS
            // send_command(sockfd, "STARTTLS\r\n", buffer);

            // Perform authentication
            state = AUTH_LOGIN;
            break;

        case AUTH_LOGIN:
            // Send AUTH LOGIN command
            send_command(sockfd, "AUTH LOGIN\r\n", buffer);
            state = AUTH_USERNAME;
            break;

        case AUTH_USERNAME:
            // Send encoded username (base64 encoded)
            send_command(sockfd, "base64_encoded_username\r\n", buffer);
            state = AUTH_PASSWORD;
            break;

        case AUTH_PASSWORD:
            // Send encoded password (base64 encoded)
            send_command(sockfd, "base64_encoded_password\r\n", buffer);
            state = SECURITY_QUESTION;
            break;

        case SECURITY_QUESTION:
            // Send security question answer
            send_command(sockfd, "Answer to security question\r\n", buffer);

            // Send MAIL FROM command
            send_command(sockfd, "MAIL FROM:<your_email@example.com>\r\n", buffer);

            // Send RCPT TO command
            send_command(sockfd, "RCPT TO:<recipient@example.com>\r\n", buffer);

            // Send DATA command
            send_command(sockfd, "DATA\r\n", buffer);

            // Send email content
            send_email(sockfd, "your_email@example.com", "recipient@example.com", "Test Email", "Hello from C SMTP client!\r\n.\r\n");

            // Quit
            send_command(sockfd, "QUIT\r\n", buffer);

            // Close socket
            close(sockfd);
            break;
    }

    return 0;
}

// Function to send an SMTP command and receive response
int send_command(int sockfd, char* command, char* response) {
    char buffer[BUFFER_SIZE];
    int bytes_sent;

    // Send command
    bytes_sent = send(sockfd, command, strlen(command), 0);
    if (bytes_sent < 0) {
        perror("Send command failed");
        return -1;
    }

    // Receive response
    if (recv(sockfd, buffer, BUFFER_SIZE, 0) < 0) {
        perror("Receive response failed");
        return -1;
    }
    printf("%s", buffer);

    // Copy response to output parameter
    strncpy(response, buffer, BUFFER_SIZE);
    return 0;
}

// Function to send an email message
int send_email(int sockfd, char* from, char* to, char* subject, char* message) {
    char email[BUFFER_SIZE];
    sprintf(email, "From: <%s>\r\nTo: <%s>\r\nSubject: %s\r\n\r\n%s", from, to, subject, message);

    // Send email content
    return send_command(sockfd, email, NULL);
}
