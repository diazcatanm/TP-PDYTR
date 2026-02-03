#!/bin/bash

echo -e "${green}Preparando servidor...${reset}"
vagrant ssh server -c "cd /home/vagrant/experimento_2/codigo; ./servidor &"

echo -e "${green}Iniciando servidor...${reset}"
sleep 5

vagrant ssh client -c "cd /home/vagrant/experimento_2/codigo; ./cliente"