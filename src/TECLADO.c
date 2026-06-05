#include <stdio.h>
#include "TECLADO.h"

void TECLADO_Config(void) {

	PINSEL_CFG_T configRow = {0};
	configRow.port = PORT_2;
	configRow.func = PINSEL_FUNC_00;
	configRow.mode = PINSEL_TRISTATE;
	configRow.openDrain = DISABLE;

	PINSEL_ConfigMultiplePins(&configRow, 3); //Configuro GPIO en P2[0] y P2[1]

    // Configura filas como salidas
    GPIO_SetDir(PORT_2, 3, GPIO_OUTPUT); // Setear como salidas
    GPIO_ClearPins(PORT_2, 3); //Forzamos a 0 para asegurar

    PINSEL_CFG_T configColumn = {0};
    configColumn.port = PORT_2;
    configColumn.func = PINSEL_FUNC_00;
    configColumn.mode = PINSEL_PULLUP;
    configColumn.openDrain = DISABLE;

    PINSEL_ConfigMultiplePins(&configColumn, (3 << 2)); //Configuto GPIO con Pull-Up en P2[2] y P2[3]
    GPIO_SetDir(PORT_2, 3 << 2, GPIO_INPUT); // Setear como entradas

    // Configuracion para las interrupciones
    GPIO_IntConfigPort(PORT_2, (3 << 2), GPIO_INT_FALLING); // Configure falling edge interruptions para columnas
    NVIC_EnableIRQ(EINT3_IRQn); // Enable external interrupt 3 for GPIO
}



//Completar con Handlers para barrido de teclado y funciones de cada boton


