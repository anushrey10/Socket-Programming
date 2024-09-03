#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    int buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Client socket created successfully.\n");

    // Fill server information
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Take array input from the user
    int num_elements;
    printf("Enter the number of elements in the array: ");
    scanf("%d", &num_elements);

    if (num_elements > BUFFER_SIZE) {
        printf("Number of elements exceeds buffer size.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Enter the elements of the array:\n");
    for (int i = 0; i < num_elements; i++) {
        scanf("%d", &buffer[i]);
    }

    // Send the integer array to the server
    sendto(sockfd, buffer, num_elements * sizeof(int), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Integer array sent to server.\n");

    // Receive the sorted array from the server
    recvfrom(sockfd, buffer, num_elements * sizeof(int), 0, NULL, NULL);

    // Display the sorted array
    printf("Sorted array received from server:\n");
    for (int i = 0; i < num_elements; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    // Close the socket
    close(sockfd);
    return 0;
}