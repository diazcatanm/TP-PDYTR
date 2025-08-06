#!/bin/bash

logmsg() {
  echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1"
}

cd /vagrant_shared/experimento2 || exit 1
mkdir -p logs/v1


port=5001

LOG="logs/v1/cliente_$(date +%Y%m%d_%H%M%S).log"

logmsg "Ejecutando hacia 192.168.1.45:5001 con buffer 3MB..." | tee -a "$LOG"
./exec/cliente_v1 192.168.1.45 $port 3145728 >> "$LOG" 2>&1

RET=$?
logmsg "Código de finalizacion $RET" | tee -a "$LOG"
