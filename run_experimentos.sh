#!/bin/bash

# --- Configuración ---
PORT=5005
REPO_DIR="/vagrant_shared"
CASES=(
    "experimento1_bloqueante/case_grande"
    "experimento1_bloqueante/case_chico"
    "experimento2_no_bloqueante/case_grande"
    "experimento2_no_bloqueante/case_chico"
)

# Diccionario de buffers
declare -A BUFFERS
BUFFERS["experimento1_bloqueante/case_grande"]=3145728
BUFFERS["experimento1_bloqueante/case_chico"]=10
BUFFERS["experimento2_no_bloqueante/case_grande"]=3145728
BUFFERS["experimento2_no_bloqueante/case_chico"]=10

logmsg() {
  echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1"
}

# 1. Levantar entorno (Equivalente a tu vagrant up original)
logmsg "Vagrant up..."
vagrant up

for CASE in "${CASES[@]}"; do
    echo "========================================================="
    logmsg "Ejecutando: $CASE"
    BYTES=${BUFFERS[$CASE]}

    # 2. Compilación (Equivalente a tu make clean; make)
    logmsg "Compilando en $CASE..."
    vagrant ssh vm1 -c "cd $REPO_DIR/$CASE && make clean && make"

    # 3. Ejecutar servidor en vm1 (Igual que servidor_v3.sh pero in-line)
    logmsg "Ejecutando servidor en vm1 con buffer $BYTES..."
    # Se usa '&' al final para que corra de fondo mientras lanzamos el cliente
    vagrant ssh vm1 -c "cd $REPO_DIR/$CASE && ./server $PORT $BYTES > log_servidor.txt 2>&1" &
    
    # 4. Sleep de espera para el servidor (Originalmente tenías 3 segundos)
    sleep 3

    # 5. Ejecutar cliente en vm2 (Igual que cliente_v3.sh)
    logmsg "Ejecutando cliente en vm2..."
    vagrant ssh vm2 -c "cd $REPO_DIR/$CASE && ./client 192.168.1.45 $PORT $BYTES > log_cliente.txt 2>&1"

    # 6. ESPERA PARA EL CASE GRANDE
    # Como el servidor.c tiene un sleep(15), debemos esperar a que termine 
    # para que el log_servidor.txt no quede incompleto.
    logmsg "Esperando a que el servidor termine de procesar..."
    sleep 17
done

logmsg "Experimento finalizado. Logs disponibles en cada carpeta."
