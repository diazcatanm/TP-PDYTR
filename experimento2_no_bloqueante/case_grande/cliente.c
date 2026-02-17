#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>
#include <time.h>
#include <errno.h>

void logmsg(const char *msg) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char ts[20];
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", t);
    printf("[%s] %s\n", ts, msg);
}

void error(const char *msg)
{
    char err_msg[256];
    snprintf(err_msg, sizeof(err_msg), "ERROR: %s - %s", msg, strerror(errno));
    logmsg(err_msg);
    exit(1);
}

int close_now(int sockfd) {
    struct linger sl = { .l_onoff = 1, .l_linger = 0 };

    if (setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &sl, sizeof(sl)) < 0) {
        perror("setsockopt");
        return -1;
    }
    logmsg("Forzando cierre de la conexion");

    // Cerrar sin esperar
    shutdown(sockfd, SHUT_RDWR); // Esto refuerza el cierre (no estrictamente necesario)
    close(sockfd);
    return 0;
}


int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3)
    {
        logmsg("ERROR: uso incorrecto - se esperaba: <host> <port> <buffer size>");
        exit(0);
    }

    int buf_size = atoi(argv[3]);

    char buffer[buf_size];
    char message[2];

    char log_line[128];
    snprintf(log_line, sizeof(log_line), "Tamaño de buffer: %d", buf_size);
    logmsg(log_line);

    // TOMA EL NUMERO DE PUERTO DE LOS ARGUMENTOS
    portno = atoi(argv[2]);

    // CREA EL FILE DESCRIPTOR DEL SOCKET PARA LA CONEXION
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET - FAMILIA DEL PROTOCOLO - IPV4 PROTOCOLS INTERNET
    // SOCK_STREAM - TIPO DE SOCKET

    if (sockfd < 0)
        error("ERROR opening socket");

    // TOMA LA DIRECCION DEL SERVER DE LOS ARGUMENTOS
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        logmsg("ERROR: no such host");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    // COPIA LA DIRECCION IP Y EL PUERTO DEL SERVIDOR A LA ESTRUCTURA DEL SOCKET
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    // DESCRIPTOR - DIRECCION - TAMAÑO DIRECCION
    //if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    bzero(buffer, buf_size);

    //**********************************************//

    // GENERA MENSAJE
    memset((buffer), 'a', buf_size);

    int cant_bytes = strlen(buffer);

    snprintf(log_line, sizeof(log_line), "Enviado cantidad de bytes del mensaje al proceso servidor (%d)", cant_bytes);
    logmsg(log_line);

    // ENVIA CANTIDAD DE BYTES DEL MENSAJE AL SOCKET
    n = write(sockfd, &cant_bytes, sizeof(cant_bytes));
    if (n < 0)
        error("ERROR writing cant bytes message to socket");

    // ESPERA RECIBIR UNA RESPUESTA
    logmsg("Esperando respuesta del proceso servidor");
    n = read(sockfd, message, 2);

    if (n < 0)
        error("ERROR reading from socket");

    //ESTABLECE COMO NO BLOQUEANTE
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
    
    // ENVIA UN MENSAJE AL SOCKET
    logmsg("Enviando mensaje al proceso servidor");
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("ERROR writing message to socket");
    bzero(buffer, buf_size);
    
    
    snprintf(log_line, sizeof(log_line), "Bytes enviados al kernel: %d", n);
    logmsg(log_line);
    
    //CIERRE ABRUPTO
    struct linger sl = { .l_onoff = 1, .l_linger = 0 };
    setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &sl, sizeof(sl));
    

    logmsg("Fin de transmisión");
    return 0;
}
