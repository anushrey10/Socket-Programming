#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUF_SIZE 1024

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    char str1[BUF_SIZE], str2[BUF_SIZE];
    socklen_t client_addr_len = sizeof(client_addr);

    // Create TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Accept a connection from the client
    if ((newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
        perror("Accept failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Receive the first string from the client
    int n = recv(newsockfd, str1, BUF_SIZE, 0);
    if (n < 0) {
        perror("Receive failed");
        close(newsockfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    str1[n] = '\0'; // Null-terminate the string

    // Receive the second string from the client
    n = recv(newsockfd, str2, BUF_SIZE, 0);
    if (n < 0) {
        perror("Receive failed");
        close(newsockfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    str2[n] = '\0'; // Null-terminate the string

    // Compare the two strings and print the result
    if (strcmp(str1, str2) == 0) {
        printf("The strings are the same.\n");
    } else {
        printf("The strings are different.\n");
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}