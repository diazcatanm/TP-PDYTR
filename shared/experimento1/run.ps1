# Colores
clear
$green = "`e[32m"
$reset = "`e[0m"

Write-Host "${green}Preparando servidor...${reset}"
Start-Process -NoNewWindow -FilePath "vagrant" -ArgumentList 'ssh server -c "cd /home/vagrant/servidor; gcc servidor.c -s -o /home/vagrant/server > /dev/null 2>&1; cd /home/vagrant; chmod +x server; ./server 8080 204800"' 
Write-Host "${green}Iniciando servidor...${reset}"
#Start-Process -NoNewWindow -FilePath "vagrant" -ArgumentList 'ssh server -c "cd /home/vagrant; chmod +x server; ./server 8080 204800"' 

Start-Sleep -Seconds 1

#Write-Host "${green}Preparando cliente...${reset}"
#Start-Process -NoNewWindow -Wait -FilePath "vagrant" -ArgumentList 'ssh client -c "cd /home/vagrant/cliente; gcc cliente.c -s -o /home/vagrant/client > /dev/null 2>&1; cd /home/vagrant; chmod +x client; ./client 192.168.56.10 8080 204800"'
#Write-Host "${green}Ejecutando cliente...${reset}"
#Start-Process -NoNewWindow -Wait -FilePath "vagrant" -ArgumentList 'ssh client -c "cd /home/vagrant; chmod +x client; ./client 192.168.56.10 8080 204800"'

Write-Host "${green}Esperando que el servidor termine...${reset}"
Start-Sleep -Seconds 2  # Ajustá si es necesario

Write-Host "${green}Experimento finalizado.${reset}"

Write-Host "---- LOG CLIENTE ----"
Get-Content cliente\logs\cliente.log

Write-Host "---- LOG SERVIDOR ----"
Get-Content servidor\logs\servidor.log