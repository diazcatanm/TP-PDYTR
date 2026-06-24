CC = gcc
CFLAGS = -Wall -g

# Definimos los nombres finales de los binarios
TARGETS = server client

all: $(TARGETS)

# Compila el servidor desde servidor.c
server: servidor.c
	$(CC) $(CFLAGS) -o server servidor.c

# Compila el cliente desde cliente.c
client: cliente.c
	$(CC) $(CFLAGS) -o client cliente.c

clean:
	rm -f $(TARGETS)
