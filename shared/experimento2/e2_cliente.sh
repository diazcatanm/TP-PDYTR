#!/bin/bash
cd /vagrant_shared/experimento2 || exit 1
mkdir -p logs

make

LOG="logs/cliente_$(date +%Y%m%d_%H%M%S).log"

echo "[Cliente Experimento 2] Ejecutando hacia 192.168.56.100:3999 con buffer 3MB..." | tee -a "$LOG"
./cliente 192.168.56.100 3999 3145728 >> "$LOG" 2>&1

RET=$?
echo "Código de finalizacion $RET" | tee -a "$LOG"
