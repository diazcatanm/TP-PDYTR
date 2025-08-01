#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE (1024 * 200)
#define DELAY (0 * 60) // segundos

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char *dinamic_buffer;
    int n;
    int buffersize = 0;
    int bytes_count;
    char *ptr = dinamic_buffer; //Puntero secundario al buffer
    FILE *log = fopen("servidor/logs/servidor.log", "w");
    fprintf(log, "Enviando mensaje de saludo...\n");
    fprint("Iniciando servidor...\n");


    if (argc < 3)
        error("ERROR bad arguments");

    //CREA EL FILE DESCRIPTOR DEL SOCKET PARA LA CONEXION
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //AF_INET - FAMILIA DEL PROTOCOLO - IPV4 PROTOCOLS INTERNET
    //SOCK_STREAM - TIPO DE SOCKET 

    if (sockfd < 0) 
    error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    //ASIGNA EL PUERTO PASADO POR ARGUMENTO
    //ASIGNA LA IP EN DONDE ESCUCHA (SU PROPIA IP)
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    //VINCULA EL FILE DESCRIPTOR CON LA DIRECCION Y EL PUERTO
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) 
            error("ERROR on binding");
            
    //SETEA LA CANTIDAD QUE PUEDEN ESPERAR MIENTRAS SE MANEJA UNA CONEXION		  
    listen(sockfd,5);
    
    // SE BLOQUEA A ESPERAR UNA CONEXION
    fprintf(log, "Esperando conexión...\n");
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, 
                &clilen);
                
    //DEVUELVE UN NUEVO DESCRIPTOR POR EL CUAL SE VAN A REALIZAR LAS COMUNICACIONES
    if (newsockfd < 0) 
        error("ERROR on accept");

    fprintf(log, "Conexión aceptada. Esperando mensaje de sincronización...\n");

    //CREO EL BUFFER DINAMICO
    buffersize = atoi(argv[2]);
    dinamic_buffer = (char *) malloc(buffersize);

    bzero(dinamic_buffer, buffersize);


    //RESPONDE AL CLIENTE
    n = write(newsockfd,"OK", 2);
    fprintf(log, "WRITE %i BYTES\n", n);
    if (n < 0) error("ERROR writing to socket");

    // Paso 2: El servidor entra en un delay antes de recibir los datos
    fprintf(log, "Entrando en delay de %d segundos...\n", DELAY);
    fflush(log);
    sleep(DELAY);

    // Paso 3 y 4: Recibe al menos 3MB de datos después del delay
    bytes_count = 0; ptr = dinamic_buffer;
    bzero(dinamic_buffer, buffersize);
    while (bytes_count < buffersize) {
        n = read(newsockfd, ptr, buffersize);
        fprintf(log, "READ %i BYTES\n", n);
        if (n < 0)
            error("ERROR reading from socket");
        bytes_count = bytes_count + n;
        ptr = ptr + n;
    }
    fprintf(log, "Datos recibidos después del delay: %d bytes\n", bytes_count);

    free(dinamic_buffer);
    return 0;
}