/* process1.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 65432
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Process 1: Waiting for connection...\n");
    
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Process 1: Connected\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);  // Reset buffer
        int valread = read(new_socket, buffer, BUFFER_SIZE - 1);
        if (valread <= 0) break;

        buffer[valread] = '\0';  // Null terminate received string
        printf("Process 1: Received string: %s\n", buffer);

        strcat(buffer, "A");  // Append 'A'
        printf("Process 1: Appended 'A': %s\n", buffer);
	sleep(1);  // Delay before sending
        send(new_socket, buffer, strlen(buffer), 0);  // Send back
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

