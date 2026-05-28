#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_exti.h" // Included but we are not sure if we will use it

void TECLADO_Config(void) {
    // Configure rows as outputs
    for (int i = 0; i < 4; i++) {
        // Pins P0.0 to P0.3 for rows
        GPIO_SetDir(0, (1 << i), GPIO_DIR_OUTPUT); // Set as output
    }

    // Configure columns as inputs with pull-up resistors
    // (THE RESISTORS AREN'T CONFIGURED YET, THEY NEED TO IMPLEMENT A PINSEL CONFIGURATION)
    for (int j = 4; j < 8; j++) {
        // Pins P0.4 to P0.7 for columns
        GPIO_SetDir(0, (1 << j), GPIO_DIR_INPUT); // Set as input
    }

    // Configurations for the interruptions
    GPIO_IntConfigPort(PORT_0, 0, GPIO_INT_FALLING); // Configure falling edge interrupt for column 0 (P0.4)
    NVIC_EnableIRQ(EINT3_IRQn); // Enable external interrupt 3 for GPIO
}


