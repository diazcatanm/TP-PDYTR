#!/bin/bash

make clean -C ./shared/experimento2
make -C ./shared/experimento2

echo "Vagrant up..."
vagrant up

echo "Ejecutando servidor en vm1..."
vagrant ssh vm1 -c "cd /vagrant_shared/experimento2 && bash ./scripts/servidor_v1.sh" &
sleep 3

echo "Ejecutando cliente en vm2..."
vagrant ssh vm2 -c "cd /vagrant_shared/experimento2 && bash ./scripts/cliente_v1.sh"

echo "Experimento 2 finalizado. Logs disponibles en shared/experimento2/logs/v1"
