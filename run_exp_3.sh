#!/bin/bash

echo -e "${green}Preparando servidor...${reset}"
vagrant ssh server -c "cd /home/vagrant/experimento_3/codigo; ./servidor &" 

echo -e "${green}Iniciando servidor...${reset}"
sleep 3

vagrant ssh client -c "cd /home/vagrant/experimento_3/codigo; ./cliente"