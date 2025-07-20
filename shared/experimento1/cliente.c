#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE (1024 * 200) // 1KB
#define DATA_SIZE (3 * 1024 * 1024) // 3 MB

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    FILE *log = fopen("logs/client.log", "w");

    // Paso 1: Sincronización entre cliente y servidor (envía mensaje de inicio)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Envía mensaje de sincronización
    fprintf(log, "Enviando mensaje de saludo\n");
    strcpy(buffer, "SYNC");
    send(sock, buffer, strlen(buffer), 0);

    // Paso 3: El cliente envía datos mientras el servidor se encuentra en delay
    // Paso 4: Envía al menos 3MB de datos
    int sent = 0;
    memset(buffer, 'A', BUFFER_SIZE);
    while (sent < DATA_SIZE) {
        printf("ITERATION\n");
        fprintf(log, "\tBlock sent\n");
        int to_send = BUFFER_SIZE;
        if (DATA_SIZE - sent < BUFFER_SIZE)
            to_send = DATA_SIZE - sent;
        send(sock, buffer, to_send, 0);
        sent += to_send;
    }
    fprintf(log, "Cliente: Enviados %d bytes\n", sent);

    close(sock);
    return 0;
}