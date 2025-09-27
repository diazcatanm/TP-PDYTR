#!/bin/bash

GREEN="\033[0;32m"
RESET="\033[0m"

echo -e "${GREEN}Ejecutando el servidor...${RESET}"
gcc -o servidor servidor.c
./servidor &

echo -e "${GREEN}Esperando que el servidor esté listo...${RESET}"
sleep 10

echo -e "${GREEN}Ejecutando el cliente...${RESET}"
gcc -o cliente cliente.c
./cliente

rm cliente servidor

echo -e "${GREEN}Finalizado${RESET}"