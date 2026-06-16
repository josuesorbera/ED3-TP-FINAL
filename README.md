# ED3-TP-FINAL
Trabajo práctico final de Electronica Digital 3 del grupo The rolling clocks - FCEFyN 2026 

# Integrantes:
- Acosta Facundo
- Caldera Pedro
- Sorbera Josué

# Descripción del proyecto:
El proyecto consiste en el diseño e implementación de un modelo de sistema de vigilancia utilizando la placa LPC1769.
El sistema está compuesto por un servomotor que mueve la cámara en un ángulo de 180°, iniciando en la posición neutra de 90°. Se comanda por un joystick que por medio del movimiento en el eje x controla el giro del servomotor.
Cuenta además con un teclado configurado para encender el sistema, apagar el sistema e incluso dejar fija la posición de la cámara.
También se incluye una pantalla LCD que trabaja en conjunto con el teclado, para mostrar el estado en el que se encuentra el sistema.
Por último, mediante comunicación UART, se podrá visualizar en computadora datos enviados cada 2 segundos que indican el valor del ADC (entre 0 y 4095) y el valor del PWM (entre 500 y 2500 us) para tener información en tiempo real.

# Hardware y Periféricos a utilizar:
- Placa de desarrollo LPC1769
- Módulo de Joystick: sensor de movimiento por medio del uso del periférico ADC
- Servomotor SG90: uso de matchs del Timer para la implementación de PWM y un pin GPIO para la señal de control
- Teclado matricial 4x4: uso del GPIO
- Módulo Pantalla LCD 1602A con chip adaptador PCF8574: para uso de comunicación I2C
- Cable USB-mini a USB 2.0: uso de comunicación UART y alimentación de la placa
- Protoboard y cables de conexión
- Fuente de alimentación externa
- 2 LEDs de colores

