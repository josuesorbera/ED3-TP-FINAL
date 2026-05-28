# ED3-TP-FINAL
Trabajo práctico final de Electronica Digital 3 del grupo The rolling clocks - FCEFyN 2026 

# Integrantes:
- Acosta Facundo
- Caldera Pedro
- Sorbera Josué

# Descripción del proyecto:
El proyecto consiste en el diseño e implementación de un sistema de vigilancia utilizando el microcontrolador LPC179. El sistema está compuesto por un servomotor, el cual se comanda por un joystick, que permite un movimiento en el eje x, además de contar con un teclado configurado para funciones varias. También se incluye una pantalla LCD donde se puede visualizar información relevante al sistema.
Por último, mediante comunicación UART, se podrá visualizar por pantalla una interfaz gráfica sobre el estado del sistema.

# Hardware y Periféricos a utilizar:
- Placa de desarrollo LPC1769
- Módulo de Joystick: uso del periférico ADC
- Servomotor SG90: uso del TIMER y un pin GPIO
- Teclado matricial 4x4: uso del GPIO
- Módulo Pantalla LCD 1602A con Chip adaptador I2C: uso de comunicación I2C
- 2 LEDs de colores
- Adaptador USB a UART CP2102: uso de comunicación UART
