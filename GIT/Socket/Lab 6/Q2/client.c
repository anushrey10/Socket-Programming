#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    int arr[] = {3, 5, 10, 15, 7, 11, 18, 19}; // Sample array
    int n = sizeof(arr) / sizeof(arr[0]);
    char buffer[BUF_SIZE];
    int prime_count;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use server's IP address here
    server_addr.sin_port = htons(PORT);

    // Send the size of the array
    if (sendto(sockfd, &n, sizeof(n), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send the array to the server
    if (sendto(sockfd, arr, n * sizeof(int), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Receive the response (number of prime numbers) from the server
    socklen_t server_addr_len = sizeof(server_addr);
    if (recvfrom(sockfd, &prime_count, sizeof(prime_count), 0, (struct sockaddr *)&server_addr, &server_addr_len) < 0) {
        perror("recvfrom failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Number of prime numbers in the array: %d\n", prime_count);

    close(sockfd);
    return 0;
}