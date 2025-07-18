#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


void error(char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno, clilen;
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  if (argc < 3)
  {
    fprintf(stderr, "ERROR, no port or buffer size provided\n");
    exit(1);
  }

  int buf_size = atoi(argv[2]);

  printf("tamaño de buffer: %d \n", buf_size);

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
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd,
                     (struct sockaddr *)&cli_addr,
                     &clilen);

  // DEVUELVE UN NUEVO DESCRIPTOR POR EL CUAL SE VAN A REALIZAR LAS COMUNICACIONES
  if (newsockfd < 0)
    error("ERROR on accept");
  bzero(buffer, buf_size);

  // LEE CANT DE BYTES QUE RECIBIRA
  int cant_bytes;
  n = read(newsockfd, &cant_bytes, sizeof(int));
  printf("cant bytes: %d\n", cant_bytes);

  bzero(buffer, buf_size);

  // RESPONDE AL CLIENTE
  n = write(newsockfd, "ok", 2);
  bzero(buffer, buf_size);

  sleep(5); 

  // LEE EL MENSAJE DEL CLIENTE
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

  return 0;
}
