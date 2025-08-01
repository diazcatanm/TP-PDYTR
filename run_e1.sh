#!/bin/bash

# Colores para el output
GREEN='\033[0;32m'
NC='\033[0m'

echo "Vagrant up..."
vagrant up

echo "${GREEN}Iniciando servidor...${NC}"
vagrant ssh server -c "cd /home/vagrant/servidor && ./server 8080 204800 &" &
sleep 3

echo "Ejecutando cliente en vm2..."
vagrant ssh vm2 -c "cd /vagrant_shared/experimento2 && bash ./e2_cliente.sh"

echo "Experimento 2 finalizado. Logs disponibles en shared/experimento2/logs/"
