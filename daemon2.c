/* process2.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 65432
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
    printf("Process 2: Connected to Process 1\n");

    strcpy(buffer, "B");  // Start with "B"
    
    while (1) {
        printf("Process 2: Sending string: %s\n", buffer);
	sleep(1);  // Delay before sending
        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUFFER_SIZE);  // Reset buffer
        int valread = read(sock, buffer, BUFFER_SIZE - 1);
        if (valread <= 0) break;

        buffer[valread] = '\0';  // Null terminate received string
        printf("Process 2: Received string: %s\n", buffer);

        strcat(buffer, "B");  // Append 'B' before next send
    }

    close(sock);
    return 0;
}

