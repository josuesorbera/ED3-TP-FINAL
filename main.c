#include <stdio.h>
#include "LPC17xx.h"

int main(void) {
    // Still needs proper configuration and add more details
    // Call the configuration functions for each peripheral
    UART_Config();
    TECLADO_Config();
    TIMER_PWM_Config();
    ADC_Config();
    DMA_Config();

    while (1) {
    }

    return 0;
}

void EINT3_IRQHandler(void){
    // Handle the external interrupt for the keypad here
    // This function will be called when a key is pressed (falling edge on column pins)
    // You can read the state of the rows to determine which key was pressed
}


