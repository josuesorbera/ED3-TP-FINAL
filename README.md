# ED3-TP-FINAL
Trabajo Práctico Final de Electrónica Digital 3 del grupo The Rolling Clocks - FCEFyN 2026

---

## 👀 Vista general del proyecto:

Este repositorio contiene:

- Librería con drivers CMSIS del fabricante(`CMSISv2p00_LPC17xx/`)
- Carpeta include con archivos .h de cabecera (`include/`)
- Carpeta src con archivos .c (de cada módulo y del main) (`src/`)
- Archivo .clang-format para estandarizar el formato del código (`.clang-format`)
- Archivo ejecutable de python que implementa una interfaz gráfica para visualizar los datos enviados por UART (`visualizador.py`)
- Diagrama de control

---

## 📓 Descripción del proyecto:

&emsp; &emsp; El proyecto consiste en el diseño e implementación de un modelo de sistema de vigilancia utilizando la placa LPC1769.
Está compuesto por un servomotor que mueve la cámara en un ángulo de 180°, iniciando en la posición neutra de 90°. Se comanda por un joystick que por medio del movimiento en el eje x controla el giro del servomotor.

&emsp; &emsp; Cuenta además con un teclado configurado para habilitar el movimiento de la cámara, dejar fija la posición de la cámara y encender una luz de alarma.
También se incluye una pantalla LCD que trabaja en conjunto con el teclado, para mostrar el estado en el que se encuentra el sistema. Cada botón dispara, al momento de accionarse, un mensaje utilizando el protocolo I2C que se muestra en la pantalla LCD.

&emsp; &emsp; Por último, mediante comunicación UART, se podrá visualizar en computadora datos enviados cada 2 segundos que indican el valor del ADC (entre 0 y 4095) y el valor del PWM (entre 500 y 2500 us) para tener información en tiempo real del sistema.
Como agregado, le incluimos un script de python que es una interfaz gráfica que muestra en tiempo real datos de los valores de ADC, PWM, ángulo de la cámara y la posición en porcentaje.

---

## 🧰 Hardware y Periféricos utilizados:

- Placa de desarrollo LPC1769
- Módulo de Joystick: sensor de movimiento por medio del uso del periférico ADC
- Servomotor SG90: uso de matches del Timer para la implementación de PWM y un pin GPIO para la señal de control
- Teclado matricial 4x4: uso del GPIO
- Módulo Pantalla LCD 1602A con chip adaptador PCF8574: para uso de comunicación I2C
- Cable USB-mini a USB 2.0: uso de comunicación UART y alimentación de la placa
- Protoboard y cables de conexión
- Fuente de alimentación externa
- 1 LED de color rojo: uso de GPIO
- 1 Resistencia de 220 ohmios: para limitar la corriente del LED

---

## 📊 Diagrama de control:


---

## 💻 Softwares de utilizados:
 
- MCUXpresso IDE: `v25.6.136`
- TeraTerm: `v5.6.1
- Python: `v3.13`

---

## 🧑‍🎓 Integrantes:
- **Acosta Facundo (DNI: 44272854)**
- **Caldera Pedro (DNI: 46377648)**
- **Sorbera Josué (DNI: 43142923)**

---

## 📚 Documentación consultada

- Datasheet y User Manual LPC1769
- Datasheet de componentes (Servomotor, LCD, etc.)