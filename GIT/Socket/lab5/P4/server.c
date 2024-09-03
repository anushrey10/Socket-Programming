#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to calculate the sum of elements in an array
int calculateSum(int* array, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int buffer[BUFFER_SIZE];
    int size, sum;

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

    // Calculate the sum of the array elements
    sum = calculateSum(buffer, size);
    printf("Sum of array elements: %d\n", sum);

    // Send the sum back to the client
    send(new_socket, &sum, sizeof(sum), 0);

    // Close the socket
    close(new_socket);
    close(server_fd);
    return 0;
}