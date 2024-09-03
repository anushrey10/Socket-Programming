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
    int matrix[3][3];
    int diag_sum = 0;
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

    // Receive the matrix from the client
    if (recv(newsockfd, matrix, sizeof(matrix), 0) < 0) {
        perror("Receive failed");
        close(newsockfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Display the received matrix
    printf("Received 3x3 matrix:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", matrix[i][j]);
            if (i == j) {
                diag_sum += matrix[i][j]; // Calculate the sum of main diagonal elements
            }
        }
        printf("\n");
    }

    // Send the sum of the main diagonal elements back to the client
    if (send(newsockfd, &diag_sum, sizeof(diag_sum), 0) < 0) {
        perror("Send failed");
        close(newsockfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}