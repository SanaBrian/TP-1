# TP1
## Controlador de temperatura

**Alumno:** Ing. Brian Sanabria

**Objetivo:** Controlar la temperatura de cámara térmica

**Descripción:**

El sistema debe controlar la temperatura dentro de una cámara térmica y mantenerla dentro de una
temperatura máxima y mínima preestablecida.
Cada 20 minutos deberá enviar un registro de temperatura a la PC.
Mientras la temperatura se mantenga dentro de los límites se mantendrá encendido un LED verde.
Cuando la temperatura sea menor a la mínima se activará una resistencia calefactora para
aumentarla hasta que alcance la temperatura promedio ([Tmax + Tmin]/2). Si la temperatura
alcanza el máximo se activará un ventilador y la temperatura disminuirá hasta alcanzar la promedio
Un LED rojo indicará que superó la Tmax. Y un LED azul indicará que la temperatura alcanzó la
Tmin.

**Plataforma de desarrollo:** NUCLEO-F429ZI

**Periféricos a utilizar:**

* ANALOG IN 1: Se utiliza para leer la temperatura de la cámara.
* LED 1: Se enciende mientras la temperatura se encuentra dentro de los límites.
* LED 2: Se enciende cuando el sistema enciende la resistencia calefactora.
* LED 3: Se enciende cuando el sistema enciende el ventilador.
* UART: Se utiliza para enviar un registro de temperatura a la PC

**Diagrama en bloques:**

<image src="https://github.com/SanaBrian/TP1/blob/master/Esquema_tp1.PNG" alt="Esquema">
