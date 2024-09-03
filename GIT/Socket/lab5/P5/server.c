#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to find the second largest element in an array
int findSecondLargest(int* array, int size) {
    if (size < 2) {
        return -1; // Not enough elements
    }

    int first = -2147483648; // Minimum integer value
    int second = -2147483648;

    for (int i = 0; i < size; i++) {
        if (array[i] > first) {
            second = first;
            first = array[i];
        } else if (array[i] > second && array[i] < first) {
            second = array[i];
        }
    }

    return (second == -2147483648) ? -1 : second; // Return -1 if no second largest found
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int buffer[BUFFER_SIZE];
    int size, secondLargest;

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

    // Read the size of the array from the client
    read(new_socket, &size, sizeof(size));
    
    // Read the array elements from the client
    read(new_socket, buffer, sizeof(int) * size);
    
    printf("Received array from client:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    // Find the second largest element in the array
    secondLargest = findSecondLargest(buffer, size);
    if (secondLargest == -1) {
        printf("There is no second largest element.\n");
    } else {
        printf("Second largest element: %d\n", secondLargest);
    }

    // Send the second largest element back to the client
    send(new_socket, &secondLargest, sizeof(secondLargest), 0);

    // Close the socket
    close(new_socket);
    close(server_fd);
    return 0;
}