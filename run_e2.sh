#!/bin/bash

echo "Vagrant up..."
vagrant up

echo "Ejecutando servidor en vm1..."
vagrant ssh vm1 -c "cd /vagrant_shared/experimento2 && bash ./e2_servidor.sh" &
sleep 3

echo "Ejecutando cliente en vm2..."
vagrant ssh vm2 -c "cd /vagrant_shared/experimento2 && bash ./e2_cliente.sh"

echo "Experimento 2 finalizado. Logs disponibles en shared/experimento2/logs/"
