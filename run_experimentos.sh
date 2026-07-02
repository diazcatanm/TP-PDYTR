#!/bin/bash
# --- Configuración ---
PORT=5005
REPO_DIR="/vagrant_shared"
CASES=(
    "experimento1_bloqueante/caso_grande"
    "experimento1_bloqueante/caso_chico"
    "experimento2_no_bloqueante/caso_grande"
    "experimento2_no_bloqueante/caso_chico"
)

declare -A BUFFERS
BUFFERS["experimento1_bloqueante/caso_grande"]=3145728
BUFFERS["experimento1_bloqueante/caso_chico"]=10
BUFFERS["experimento2_no_bloqueante/caso_grande"]=3145728
BUFFERS["experimento2_no_bloqueante/caso_chico"]=9216

logmsg() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1"
}

logmsg "Vagrant up..."
vagrant up

for CASE in "${CASES[@]}"; do
    echo "========================================================="
    logmsg "Ejecutando: $CASE"
    BYTES=${BUFFERS[$CASE]}

    # Verificar que existan los archivos antes de hacer cualquier cosa
    if ! vagrant ssh vm1 -c "test -f $REPO_DIR/$CASE/cliente.c && test -f $REPO_DIR/$CASE/servidor.c && test -f $REPO_DIR/$CASE/Makefile" 2>/dev/null; then
        logmsg "SALTANDO $CASE: faltan archivos (cliente.c, servidor.c o Makefile)"
        continue
    fi

    logmsg "Compilando en $CASE..."
    vagrant ssh vm1 -c "cd $REPO_DIR/$CASE && make clean; make"

    # Iniciar tcpdump en background en vm1
    logmsg "Iniciando tcpdump en vm1..."
    vagrant ssh vm1 -c "sudo tcpdump -i enp0s8 port $PORT -n > $REPO_DIR/$CASE/tcpdump.log 2>&1" &
    sleep 1  # darle tiempo a tcpdump a arrancar

    logmsg "Ejecutando servidor en vm1 con buffer $BYTES..."
    vagrant ssh vm1 -c "cd $REPO_DIR/$CASE && ./server $PORT $BYTES > log_servidor.txt 2>&1" &

    sleep 3

    logmsg "Ejecutando cliente en vm2..."
    vagrant ssh vm2 -c "cd $REPO_DIR/$CASE && ./client 192.168.1.45 $PORT $BYTES > log_cliente.txt 2>&1"

    logmsg "Esperando a que el servidor termine de procesar..."
    sleep 17

    # Matar tcpdump
    logmsg "Deteniendo tcpdump..."
    vagrant ssh vm1 -c "sudo pkill tcpdump" || true
    sleep 1

    logmsg "=== LOG SERVIDOR ($CASE) ==="
    vagrant ssh vm1 -c "cat $REPO_DIR/$CASE/log_servidor.txt" || true
    logmsg "=== LOG CLIENTE ($CASE) ==="
    vagrant ssh vm2 -c "cat $REPO_DIR/$CASE/log_cliente.txt" || true
    logmsg "=== TCPDUMP ($CASE) ==="
    vagrant ssh vm1 -c "cat $REPO_DIR/$CASE/tcpdump.log" || true
done

logmsg "Experimento finalizado. Logs disponibles en cada carpeta."
