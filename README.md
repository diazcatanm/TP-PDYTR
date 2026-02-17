# TP-PDYTR: Sockets en C y Análisis de Comportamiento TCP

En este proyecto se analiza el comportamiento de las comunicaciones de red mediante el uso de sockets bloqueantes y no bloqueantes. El objetivo principal es la verificación de fallos en la transmisión y la detección de inconsistencias en el protocolo.

---

## Requisitos Previos

Para la ejecución del proyecto, es requerida la instalación de los siguientes componentes en la máquina host:

* **Vagrant**: Es utilizado para la gestión de entornos virtuales.
* **VirtualBox**: Es empleado como proveedor de virtualización.

---

## Instrucciones de Ejecución

### 1. Preparación del Envío
Desde la raíz del repositorio, las máquinas virtuales son iniciadas mediante el siguiente comando:

`vagrant up`

* **VM1 (Servidor)**: Es configurada con la IP estática **192.168.1.45**.
* **VM2 (Cliente)**: Una IP dinámica es asignada automáticamente por el sistema para la salida a red pública.



### 2. Automatización de Experimentos
La compilación de cada caso, la limpieza de puertos y la recolección de trazas son gestionadas por el script `run_experimentos.sh`. Los permisos son otorgados y el proceso es iniciado de la siguiente manera:

```bash
chmod +x run_all.sh
./run_experimentos.sh
```

## Análisis de Resultados

Dos archivos de log independientes son generados dentro de la carpeta de cada experimento para permitir un análisis pormenorizado:

* log_servidor.txt: La recepción de bytes, los tiempos de delay y los fallos de red detectados son registrados en este archivo.

* log_cliente.txt: El volumen de datos entregado satisfactoriamente al kernel y el estado final de la transmisión son documentados aquí.

## Estructura del Proyecto

La integridad de las rutas es mantenida bajo el siguiente esquema:
/TP-PDYTR
├── /experimento1_bloqueante # Casos con sockets bloqueantes.
├── /experimento2_no_bloqueante # Casos con sockets no bloqueantes.
├── Vagrantfile # Configuración de red de las máquinas.
├── run_experimentos.sh # Script de orquestación de experimentos.
└── README.md
