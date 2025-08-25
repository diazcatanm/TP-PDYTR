#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#define SERVER_IP "192.168.1.45"
#define PORT 8081
#define BUFFER_SIZE (1024 * 200) // 1KB
#define DATA_SIZE (3 * 1024 * 1024) // 3 MB

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    FILE *log = fopen("/home/vagrant/experimento_2/logs/cliente.log", "w");

    fprintf(log, "Estableciendo conexion con el servidor...\n");
    fflush(log);

    // Paso 1: Sincronización entre cliente y servidor (envía mensaje de inicio)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Socket en modo no bloqueante
    fcntl(sock, F_SETFL, O_NONBLOCK);
    fprintf(log, "Conexion establecida\n\n");
    fflush(log);

    // Paso 3: El cliente envía datos mientras el servidor se encuentra en delay
    int sent = 0; int n = 0;
    memset(buffer, 'A', BUFFER_SIZE);
    while (sent < DATA_SIZE) {
        int to_send = BUFFER_SIZE;
        if (DATA_SIZE - sent < BUFFER_SIZE)
            to_send = DATA_SIZE - sent;

        int total_sent = 0;
        while (total_sent < to_send) {
            n = send(sock, buffer + total_sent, to_send - total_sent, 0);
            if (n > 0) {
                total_sent += n;
                fprintf(log, "\tBloque enviado: %d bytes\n", n);
            } else if (n < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    fprintf(log, "Error: buffer de transmisión lleno (EAGAIN/EWOULDBLOCK)\n");
                    perror("send");
                    break; // Sale del while interno, intenta el siguiente bloque después del sleep
                } else {
                    perror("send");
                    exit(EXIT_FAILURE);
                }
            }
            fflush(log);
            sleep(10); // Simulate some delay in sending
        }
        sent += n;
    }
    fprintf(log, "Cliente: Enviados %d bytes\n", sent);

    close(sock);
    return 0;
}