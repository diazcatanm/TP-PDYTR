#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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

int main(int argc, char *argv[])
{
  //int sockfd, newsockfd, portno, clilen;
  struct sockaddr_in serv_addr, cli_addr;
  int sockfd, newsockfd, portno;
  socklen_t clilen;

  int n;

  if (argc < 3)
  {
    logmsg("ERROR: uso incorrecto - se esperaba: <port> <buffer size>");
    exit(1);
  }

  int buf_size = atoi(argv[2]);

  char log_line[128];
  snprintf(log_line, sizeof(log_line), "Tamaño de buffer: %d", buf_size);
  logmsg(log_line);

  char buffer[buf_size];

  // CREA EL FILE DESCRIPTOR DEL SOCKET PARA LA CONEXION
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  // AF_INET - FAMILIA DEL PROTOCOLO - IPV4 PROTOCOLS INTERNET
  // SOCK_STREAM - TIPO DE SOCKET

  if (sockfd < 0)
    error("ERROR opening socket");
  bzero((char *)&serv_addr, sizeof(serv_addr));
  // ASIGNA EL PUERTO PASADO POR ARGUMENTO
  // ASIGNA LA IP EN DONDE ESCUCHA (SU PROPIA IP)
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  // VINCULA EL FILE DESCRIPTOR CON LA DIRECCION Y EL PUERTO
  if (bind(sockfd, (struct sockaddr *)&serv_addr,
           sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  // SETEA LA CANTIDAD QUE PUEDEN ESPERAR MIENTRAS SE MANEJA UNA CONEXION
  listen(sockfd, 5);

  // SE BLOQUEA A ESPERAR UNA CONEXION
  logmsg("Esperando conexion");
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd,
                     (struct sockaddr *)&cli_addr,
                     &clilen);

  // DEVUELVE UN NUEVO DESCRIPTOR POR EL CUAL SE VAN A REALIZAR LAS COMUNICACIONES
  if (newsockfd < 0)
    error("ERROR on accept");
  bzero(buffer, buf_size);
  
  logmsg("Conexion establecida");

  // LEE CANT DE BYTES QUE RECIBIRA
  logmsg("Recibiendo cantidad de bytes del mensaje a leer");
  int cant_bytes;
  n = read(newsockfd, &cant_bytes, sizeof(int));
  snprintf(log_line, sizeof(log_line), "Cantidad de bytes del mensaje: %d", cant_bytes);
  logmsg(log_line);

  bzero(buffer, buf_size);

  // RESPONDE AL CLIENTE
  logmsg("Confirmando recepcion al proceso cliente");
  n = write(newsockfd, "ok", 2);
  bzero(buffer, buf_size);

  logmsg("Iniciando 5 segundos de delay");
  sleep(5); 

  // LEE EL MENSAJE DEL CLIENTE
  logmsg("Leyendo mensaje en Buffer");
  int j = 0;
  do
  {
    n = read(newsockfd, &buffer[j], cant_bytes);
    if (n < 0)
    {
      error("ERROR reading from socket");
      break;
    }
    j += n;
  } while (j < cant_bytes);

  logmsg("Fin de la recepcion");
  return 0;
}
