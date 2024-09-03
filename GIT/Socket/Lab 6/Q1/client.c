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
    char buffer[BUF_SIZE];
    char response[BUF_SIZE];

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

    while (1) {
        // Get the string from the user
        printf("Enter a string to send to the server (or 'exit' to quit): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Send the string to the server
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            perror("sendto failed");
            continue;
        }

        // Receive the response from the server
        socklen_t server_addr_len = sizeof(server_addr);
        int recv_len = recvfrom(sockfd, response, BUF_SIZE - 1, 0, (struct sockaddr *) &server_addr, &server_addr_len);
        if (recv_len < 0) {
            perror("recvfrom failed");
            continue;
        }
        response[recv_len] = '\0';
        printf("Server response: %s\n", response);
    }

    close(sockfd);
    return 0;
}