$green = "`e[0;32m"
$reset = "`e[0m"

Write-Host "${green}Ejecutando el servidor en vm1 (servidor)...${reset}"
vagrant ssh vm1 -c "cd /vagrant_code; nohup ./servidor > logs/vm1_server.out 2>&1 &"

Write-Host "${green}Esperando que el servidor esté listo...${reset}"
Start-Sleep -Seconds 10

Write-Host "${green}Ejecutando el cliente en vm2 (cliente)...${reset}"
vagrant ssh vm2 -c "cd /vagrant_code; nohup ./cliente > logs/vm2_client.out 2>&1"