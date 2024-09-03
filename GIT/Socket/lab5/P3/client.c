#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char message[1024];
    int palindromeCheck;

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Input the string from the user
    printf("Enter a string: ");
    fgets(message, 1024, stdin);
    message[strcspn(message, "\n")] = 0; // Remove newline character

    // Send the string to the server
    send(sock, message, strlen(message), 0);
    printf("String sent to server: %s\n", message);

    // Receive the palindrome check result from the server
    read(sock, &palindromeCheck, sizeof(palindromeCheck));
    if (palindromeCheck) {
        printf("The string is a palindrome.\n");
    } else {
        printf("The string is not a palindrome.\n");
    }

    // Close the socket
    close(sock);
    return 0;
}