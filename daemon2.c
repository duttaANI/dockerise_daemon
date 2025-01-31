/* process2.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

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

    struct hostent *server = gethostbyname("daemon1"); // Resolve container name
    if (server == NULL) {
        perror("Host not found");
        exit(EXIT_FAILURE);
    }
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
    printf("daemon 2: Connected to daemon 1\n");

    strcpy(buffer, "B");  // Start with "B"
    
    while (1) {
        printf("daemon 2: Sending string: %s\n", buffer);
	sleep(1);  // Delay before sending
        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUFFER_SIZE);  // Reset buffer
        int valread = read(sock, buffer, BUFFER_SIZE - 1);
        if (valread <= 0) break;

        buffer[valread] = '\0';  // Null terminate received string
        printf("daemon 2: Received string: %s\n", buffer);

        strcat(buffer, "B");  // Append 'B' before next send
    }

    close(sock);
    return 0;
}

