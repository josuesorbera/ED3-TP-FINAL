#include <stdio.h>
#include "TECLADO.h"

//variables globales volatiles para flags
volatile uint8_t tecla_presionada = 0xFF;
volatile uint8_t flag_teclado = 0;

void TECLADO_Config(void) {
	SYSTICK_InternalInit(20); //configuro systick con interrupciones cada 20ms para leer teclado

	PINSEL_CFG_T configPin = {0}; //struct para configurar pines GPIO
	//configuro filas P2.0 y P2.1
	configPin.port = PORT_2;
	configPin.func = PINSEL_FUNC_00;
	configPin.openDrain = DISABLE;
	configPin.mode = PINSEL_TRISTATE; //tristate para salidas

	PINSEL_ConfigMultiplePins(&configPin, 3);
	GPIO_SetDir(PORT_2, 3, GPIO_OUTPUT); //filas como salidas

	GPIO_ClearPins(PORT_2, 3); //filas en 0V para generar el flanco de bajada al tocar

	//configuro columnas P2.2 y P2.3
	configPin.mode = PINSEL_PULLUP; //pullup para entradas

	PINSEL_ConfigMultiplePins(&configPin, (3 << 2));
	GPIO_SetDir(PORT_2, (3 << 2), GPIO_INPUT);

	GPIO_IntConfigPort(PORT_2, (3 << 2), GPIO_INT_FALLING);
	NVIC_EnableIRQ(EINT3_IRQn); //habilito interrupcion por flanco de bajada en P2.2 y P2.2
    NVIC_SetPriority(EINT3_IRQn, 1); //seteo prioridad de la interrupcion del teclado
}

//handler para cuando detecta un boton presionado
void EINT3_IRQHandler(void) {
    NVIC_DisableIRQ(EINT3_IRQn);//deshabilito interrupciones para no detectar "ruido" mientras espero al systick

    GPIO_ClearInt(PORT_2, (1 << 2)); //levanto flag de columna 1
    GPIO_ClearInt(PORT_2, (1 << 3)); //levanto flag de columna 2
    NVIC_ClearPendingIRQ(EINT3_IRQn);

    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE); //disparo systick para contar 20ms (delay antirebote)
}


void SysTick_Handler(void) {
    static uint8_t esperando_liberacion = 0; //variable para detectar si no se mantiene una tecla

    uint8_t estado_actual = 0xFF; //inicializo la variable en 0xFF para evitar lecturas incorrectas

    //escaneo fila 2
    GPIO_ClearPins(PORT_2, (1 << 0)); //fila 1 a 0V
    GPIO_SetPins(PORT_2, (1 << 1));   //fila 2 a 3.3V

    uint32_t puerto = GPIO_ReadValue(PORT_2); //lectura valor puerto 2

    if ((puerto & (1 << 2)) == 0) {
        estado_actual = 3; //columna 1
    }
    else if ((puerto & (1 << 3)) == 0) {
        estado_actual = 4; //columna 2
    }

    //escaneo fila 1
    GPIO_SetPins(PORT_2, (1 << 0));   // fila 1 a 3.3V
    GPIO_ClearPins(PORT_2, (1 << 1)); // fila 2 a 0V

	PINSEL_ConfigMultiplePins(&configRow, 3); //Configuro GPIO en P2[0] y P2[1]

    puerto = GPIO_ReadValue(PORT_2); //lectura valor puerto 2

    if ((puerto & (1 << 2)) == 0) {
        estado_actual = 1; //columna 1
    }
    else if ((puerto & (1 << 3)) == 0) {
        estado_actual = 2; //columna 2
    }

    //logica para bloqueo por pulsacion mantenida
    if (estado_actual != 0xFF) { //si se pulso un boton, ingresa al if
        if (esperando_liberacion == 0) {
            //es el primer toque, registro el dato
            tecla_presionada = estado_actual;
            flag_teclado = 1;
            esperando_liberacion = 1; //"se traba" mientra este apretado
        }

        GPIO_ClearPins(PORT_2, 3); //bajo las filas para la siguiente lectura
        //el systick seguirá corriendo y entrando acá cada 20ms, hasta que se libere la tecla
    }
    else {
        //ya se solto el boton, se limpia la flag de esperando_liberacion para el proximo pulsado
        esperando_liberacion = 0;

        SYSTICK_Cmd(DISABLE);
        SYSTICK_IntCmd(DISABLE); //desactivo el systick

        GPIO_ClearPins(PORT_2, 3);//bajo las filas para la siguiente lectura

        //limpio por las dudas, interrupciones en puerto 2
        GPIO_ClearInt(PORT_2, (1 << 2));
        GPIO_ClearInt(PORT_2, (1 << 3));
        NVIC_ClearPendingIRQ(EINT3_IRQn);

        //habilito nuevamente las interrupciones por Puerto 2 para volver a leer el teclado
        NVIC_EnableIRQ(EINT3_IRQn);


void mode_Sleep(void) { //activo modo sleep en la LPC, se despierta con cualquier interrupcion por Puerto 2
    GPIO_ClearPins(PORT_2, 3); //filas en 0 para leer botón
    CLKPWR_Sleep(); //modo sleep

    //al despertar, bucle para no realizar acción del primer boton pulsado
    while (flag_teclado == 0) {
    }
    flag_teclado = 0; //pulsacion descartada
}

void choose_Action(void) {
    if (flag_teclado == 1) {
        switch (tecla_presionada) {
            case 1 :
                ADC_PowerUp(); //encender ADC
            break;
            case 2 :
                ADC_PowerDown(); //apagar ADC
            break;
            case 3 :
                modo_Sleep(); //"apagar" LPC
            break;
        }
    }

    flag_teclado = 0; // bajo la bandera
}