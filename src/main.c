#include <stdio.h>
#include <stdlib.h>
#include "LPC17xx.h"
#include "TIMER_PWM.h"
#include "ADC.h"
#include "UART.h"
#include "TECLADO.h"
#include "I2C.h"
#include "DMA.h"

#define SAMPLES 64
static volatile uint32_t adc_buffer[SAMPLES];
volatile uint16_t adcValue = 1500; //lo usa DMA_IRQHandler
GPDMA_LLI_T lli;

int main(void) {
    // Initialize the linked list
    GPDMA_LLI_T lli = {0};
    lli.srcAddr = (uint32_t)&LPC_ADC->ADDR0;
    lli.dstAddr = DIR_VALADC;
    lli.nextLLI = (uint32_t)&lli; // Point to itself for circular mode
    lli.control = (4095 | 1 << 18 | 1 << 21); // 4095 bytes, burst size of 1, increment source address

    // Call the configuration functions for each peripheral
    UART_Config();
    TECLADO_Config();
    TIMER_PWM_Config();
    servo_GPIO();
    I2C_Config();
    LCD_Init();
	LED_Config();
    ADC_Config();
    DMA_Config(&lli);

    while(1) {
        choose_Action();
		//no mas llamado a la funcion, lo hace el DMA
        if(flag_imprimir == 1) {
            uart_send_data(adcValue, pulseWidth);
            flag_imprimir = 0;
        }
    }
    return 0;
}
