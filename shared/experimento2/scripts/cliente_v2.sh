#!/bin/bash
logmsg() {
  echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1"
}

cd /vagrant_shared/experimento2 || exit 1
mkdir -p logs/v2

port=5002

LOG="logs/v2/cliente$(date +%Y%m%d_%H%M%S).log"

logmsg "Ejecutando hacia 192.168.1.45:5002 con buffer 3MB..." | tee -a "$LOG"
./exec/cliente_v2 192.168.1.45 $port 3145728 >> "$LOG" 2>&1

RET=$?
logmsg "Código de finalizacion $RET" | tee -a "$LOG"
