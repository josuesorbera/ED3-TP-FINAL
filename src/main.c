#include <stdio.h>
#include "LPC17xx.h"

GPDMA_LLI_T lli;

int main(void) {
	//inicializar lli en 0 y luego asignarle los valores
	GPDMA_LLI_T lli = {0};
	lli.srcAddr =(uint32_t)&LPC_ADC->ADDR0;
	lli.dstAddr =(uint32_t)&LPC_DAC->DACR;
	lli.nextLLI =(uint32_t)&lli; // la apunto a si misma, luego de inicializarla
	lli.control =(4095 | 1<<18 | 1<<21); //4095 datos, dato halfword en source(18) y destination (21)

    // Call the configuration functions for each peripheral
    UART_Config();
    TECLADO_Config();
    TIMER_PWM_Config();
    I2C_Config();
    ADC_Config();
    DAC_Config();
    DMA_Config(GPDMA_LLI_T* lli);

    while (1) {
    }

    return 0;
}

void EINT3_IRQHandler(void){
    // Handle the external interrupt for the keypad here
    // This function will be called when a key is pressed (falling edge on column pins)
    // You can read the state of the rows to determine which key was pressed
}


