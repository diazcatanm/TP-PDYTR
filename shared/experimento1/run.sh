#!/bin/bash

# Colores para el output
GREEN='\033[0;32m'
NC='\033[0m'

echo -e "${GREEN}Compilando cliente y servidor...${NC}"
gcc -o client cliente.c
gcc -o server servidor.c

# Asegurarse de que el directorio de logs existe
mkdir -p logs
rm -f logs/*.log

echo -e "${GREEN}Iniciando servidor...${NC}"
./server &  # Ejecuta el servidor en segundo plano
SERVER_PID=$!

sleep 1  # Pequeño delay para asegurar que el servidor se inicie

echo -e "${GREEN}Ejecutando cliente...${NC}"
./client

echo -e "${GREEN}Esperando que el servidor termine...${NC}"
wait $SERVER_PID

echo -e "${GREEN}Experimento finalizado.${NC}"
echo "---- LOG CLIENTE ----"
cat logs/client.log

echo "---- LOG SERVIDOR ----"
cat logs/server.log