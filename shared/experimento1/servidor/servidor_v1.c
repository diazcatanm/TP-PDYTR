#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE (1024 * 100)
#define DELAY (2 * 60) // segundos

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    FILE *log = fopen("/home/vagrant/servidor/logs/server_v1.log", "w");
    
    // Paso 1: Sincronización entre cliente y servidor (espera mensaje de inicio)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 3);
    fprintf(log, "Esperando conexión...\n");
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    fprintf(log, "Conexión establecida. Esperando mensaje de sincronización...\n\n");
    fflush(log);

    // Saludos entre el cliente y el servidor
    int bytes = recv(new_socket, buffer, BUFFER_SIZE, 0);
    buffer[bytes] = '\0';
    fprintf(log, "Recibido saludo de sincronización: %s\n", buffer);
    fflush(log);
    fprintf(log, "Respondo el saludo al cliente...\n");
    strcpy(buffer, "ACK");
    send(new_socket, buffer, strlen(buffer), 0);

    // Paso 2: El servidor entra en un delay antes de recibir los datos
    fprintf(log, "Entrando en delay de %d segundos...\n", DELAY);
    fflush(log);
    sleep(DELAY);

    // Paso 3 y 4: Recibe al menos 3MB de datos después del delay
    int total = 0;
    while ((bytes = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        total += bytes;
    }
    fprintf(log, "Datos recibidos después del delay: %d bytes\n\n", total);

    fprintf(log, "Fin y cierre de socket");
    fflush(log);
    fclose(log);
    close(new_socket);
    close(server_fd);
    return 0;
}