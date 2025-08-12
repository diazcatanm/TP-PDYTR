# Colores
clear
$green = "`e[32m"
$reset = "`e[0m"

Write-Host "${green}Preparando servidor...${reset}"
Start-Process -NoNewWindow -FilePath "vagrant" -ArgumentList 'ssh server -c "cd /home/vagrant/servidor; gcc servidor_v2.c -s -o /home/vagrant/server > /dev/null 2>&1; cd /home/vagrant; chmod +x server; ./server"' 
Write-Host "${green}Iniciando servidor...${reset}"

Start-Sleep -Seconds 3

Start-Process -NoNewWindow -Wait -FilePath "vagrant" -ArgumentList 'ssh client -c "cd /home/vagrant/cliente; gcc cliente_v2.c -s -o /home/vagrant/client > /dev/null 2>&1; cd /home/vagrant; chmod +x client; ./client"'

Write-Host "${green}Experimento finalizado.${reset}"