
# TP-PDYTR: Análisis de comportamiento TCP mediante sockets en C

Este proyecto analiza el comportamiento de comunicaciones de red usando sockets bloqueantes y no bloqueantes, con el objetivo de observar y documentar fallos en la transmisión de datos sobre TCP.

---

## Requisitos

- **Vagrant** — gestión del entorno virtualizado
- **VirtualBox** — proveedor de virtualización

---

## Cómo ejecutar

```bash
chmod +x run_experimentos.sh
./run_experimentos.sh
```

El script levanta las VMs, compila cada caso, limpia el puerto entre ejecuciones, lanza servidor y cliente, y captura el tráfico de red.

---

## Logs generados

Dentro de la carpeta de cada caso:

- `log_servidor.txt` — bytes recibidos, tiempos de delay y errores de red
- `log_cliente.txt` — bytes enviados al kernel y estado final de la transmisión
- `tcpdump.log` — tráfico TCP capturado durante la ejecución

---
