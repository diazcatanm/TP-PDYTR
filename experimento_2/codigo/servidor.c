#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE (1024 * 200)
#define DELAY (15) // segundos

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    FILE *log = fopen("/home/vagrant/experimento_2/logs/servidor.log", "w");

    // Paso 1: Sincronización entre cliente y servidor (espera mensaje de inicio)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    fprintf(log, "Esperando conexión...\n");
    fflush(log);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    fprintf(log, "Conexión aceptada. Esperando mensaje de sincronización...\n");
    fflush(log);

    sleep(DELAY);
    
    // Espera mensaje de sincronización del cliente
    int bytes = recv(new_socket, buffer, BUFFER_SIZE, 0);
    buffer[bytes] = '\0';
    fprintf(log, "Recibido saludo de sincronización: %s\n", buffer);
    fflush(log);

    // Paso 2: El servidor entra en un delay antes de recibir los datos
    fprintf(log, "Entrando en delay de %d segundos...\n", DELAY);
    fflush(log);
    sleep(DELAY*2);

    // Paso 3 y 4: Recibe al menos 3MB de datos después del delay
    int total = 0;
    while ((bytes = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        total += bytes;
    }
    fprintf(log, "Datos recibidos después del delay: %d bytes\n", total);

    fclose(log);
    close(new_socket);
    close(server_fd);
    return 0;
}