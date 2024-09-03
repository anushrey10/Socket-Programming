#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUF_SIZE 1024

int is_palindrome(const char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0; // Not a palindrome
        }
    }
    return 1; // Is a palindrome
}

void handle_client(int sockfd, struct sockaddr_in *client_addr, socklen_t client_addr_len) {
    char buffer[BUF_SIZE];
    char response[BUF_SIZE];
    
    // Receive data from the client
    int recv_len = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0, (struct sockaddr *) client_addr, &client_addr_len);
    if (recv_len < 0) {
        perror("recvfrom failed");
        return;
    }
    buffer[recv_len] = '\0';
    printf("Received string: %s\n", buffer);

    if (is_palindrome(buffer)) {
        snprintf(response, sizeof(response), "The string is a palindrome.");
    } else {
        // Compute the difference between the first pair of differing characters
        int len = strlen(buffer);
        int min_length = len;
        int diff = -1;
        
        for (int i = 0; i < min_length / 2; i++) {
            if (buffer[i] != buffer[len - i - 1]) {
                diff = abs(buffer[i] - buffer[len - i - 1]);
                break;
            }
        }
        
        if (diff != -1) {
            snprintf(response, sizeof(response), "The string is not a palindrome. Difference: %d.", diff);
        } else {
            snprintf(response, sizeof(response), "The string is not a palindrome. No differing characters found.");
        }
    }
    
    // Send the response to the client
    if (sendto(sockfd, response, strlen(response), 0, (struct sockaddr *) client_addr, client_addr_len) < 0) {
        perror("sendto failed");
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

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

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is ready to receive messages.\n");

    while (1) {
        handle_client(sockfd, &client_addr, client_addr_len);
    }

    close(sockfd);
    return 0;
}