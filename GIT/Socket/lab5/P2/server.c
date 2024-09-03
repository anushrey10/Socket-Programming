#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>  // Include this header for tolower

#define PORT 8080

// Function to count the number of vowels in a string
int countVowels(const char* str) {
    int count = 0;
    while (*str) {
        char c = tolower(*str);  // Use tolower to handle uppercase vowels
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
        str++;
    }
    return count;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    int vowelCount;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Binding the socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Accept a connection
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Read the string from the client
    read(new_socket, buffer, 1024);
    printf("Received string from client: %s\n", buffer);

    // Count the number of vowels in the string
    vowelCount = countVowels(buffer);
    printf("Number of vowels: %d\n", vowelCount);

    // Send the number of vowels back to the client
    send(new_socket, &vowelCount, sizeof(vowelCount), 0);

    // Close the socket
    close(new_socket);
    close(server_fd);
    return 0;
}