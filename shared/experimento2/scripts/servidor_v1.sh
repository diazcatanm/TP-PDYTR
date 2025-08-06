#!/bin/bash
logmsg() {
  echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1"
}

cd /vagrant_shared/experimento2 || exit 1
mkdir -p logs/v1

port=5001

LOG="logs/v1/servidor_$(date +%Y%m%d_%H%M%S).log"

logmsg "Ejecutando en puerto 5001 con buffer 3MB..." | tee -a "$LOG"
./exec/servidor_v1 $port 3145728 >> "$LOG" 2>&1

RET=$?
logmsg "Código de finalizacion: $RET" | tee -a "$LOG"
