#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE (1024 * 200) // 1KB
#define DATA_SIZE (3 * 1024 * 1024) // 3 MB

int main(int argc, char *argv[]) {
    return 0;
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int buffersize = 0;
    struct timespec tv;
    char *dinamic_buffer;
    FILE *log = fopen("logs/cliente.log", "w");
    fprintf(log, "Enviando mensaje de saludo...\n");
	//TOMA EL NUMERO DE PUERTO DE LOS ARGUMENTOS
    portno = atoi(argv[2]);
	
	//CREA EL FILE DESCRIPTOR DEL SOCKET PARA LA CONEXION
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET - FAMILIA DEL PROTOCOLO - IPV4 PROTOCOLS INTERNET
	//SOCK_STREAM - TIPO DE SOCKET 
	
    if (sockfd < 0) 
        error("ERROR opening socket");
	
	//TOMA LA DIRECCION DEL SERVER DE LOS ARGUMENTOS
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

	//COPIA LA DIRECCION IP Y EL PUERTO DEL SERVIDOR A LA ESTRUCTURA DEL SOCKET
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
     serv_addr.sin_port = htons(portno);
	
	//DESCRIPTOR - DIRECCION - TAMAÑO DIRECCION
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    //CREO EL BUFFER DINAMICO
    buffersize = atoi(argv[3]);
    dinamic_buffer = (char *) malloc(buffersize);
    bzero(dinamic_buffer, buffersize);

    // Envía mensaje de sincronización
    fprintf(log, "Enviando mensaje de saludo...\n");
    strcat(dinamic_buffer, "Hola, servidor!");
    //send(sock, buffer, strlen(buffer), 0);

    //ENVIA EL ARREGLO AL SOCKET
    n = write(sockfd, dinamic_buffer, buffersize);
    fprintf(log, "WRITE %i BYTES\n", n);
    if (n < 0) 
        error("ERROR writing to socket");
        
    //ESPERA RECIBIR UNA RESPUESTA
    bzero(dinamic_buffer, buffersize);
    n = read(sockfd, dinamic_buffer, 2);
    fprintf(log, "Recibido el mensaje de saludo\n");
    fprintf(log, "READ %i BYTES\n", n);
    if (n < 0)
        error("ERROR reading from socket");
    
    int sent = 0;
    memset(dinamic_buffer, 'A', buffersize);
    while (sent < DATA_SIZE) {
        printf("ITERATION\n");
        fprintf(log, "\tBlock sent\n");
        int to_send = buffersize;
        if (DATA_SIZE - sent < buffersize)
            to_send = DATA_SIZE - sent;
        //send(sock, buffer, to_send, 0);
        n = write(sockfd, dinamic_buffer, buffersize);
        sent += n;
    }
    fprintf(log, "Cliente: Enviados %d bytes\n", sent);

    free(dinamic_buffer);
    return 0;
}