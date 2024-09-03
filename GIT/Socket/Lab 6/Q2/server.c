#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUF_SIZE 1024

int is_prime(int num) {
    if (num <= 1) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    int arr[BUF_SIZE];
    int n;
    char buffer[BUF_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        socklen_t client_addr_len = sizeof(client_addr);

        // Receive the size of the array from the client
        if (recvfrom(sockfd, &n, sizeof(n), 0, (struct sockaddr *)&client_addr, &client_addr_len) < 0) {
            perror("recvfrom failed");
            continue;
        }

        // Receive the array from the client
        if (recvfrom(sockfd, arr, n * sizeof(int), 0, (struct sockaddr *)&client_addr, &client_addr_len) < 0) {
            perror("recvfrom failed");
            continue;
        }

        // Calculate the number of prime numbers in the array
        int prime_count = 0;
        for (int i = 0; i < n; i++) {
            if (is_prime(arr[i])) {
                prime_count++;
            }
        }

        // Send the result back to the client
        if (sendto(sockfd, &prime_count, sizeof(prime_count), 0, (struct sockaddr *)&client_addr, client_addr_len) < 0) {
            perror("sendto failed");
            continue;
        }

        printf("Processed array and sent prime count: %d\n", prime_count);
    }

    close(sockfd);
    return 0;
}