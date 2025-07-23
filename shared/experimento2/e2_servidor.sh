#!/bin/bash
logmsg() {
  echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1"
}

#!cd /vagrant_shared/experimento2 || exit 1
mkdir -p logs

make

LOG="logs/servidor_$(date +%Y%m%d_%H%M%S).log"

logmsg "Ejecutando en puerto 3999 con buffer 3MB..." | tee -a "$LOG"
./servidor 3999 3145728 >> "$LOG" 2>&1

RET=$?
logmsg "Código de finalizacion: $RET" | tee -a "$LOG"
