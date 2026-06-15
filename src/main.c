#include <stdio.h>
#include "LPC17xx.h"
#include "TIMER_PWM.h"
#include "ADC.h"
#include "UART.h"
#include "TECLADO.h"
#include "I2C.h"
#include "DMA.h"

GPDMA_LLI_T lli;

int main(void) {
    // inicializar lli en 0 y luego asignarle los valores
    GPDMA_LLI_T lli = {0};
    lli.srcAddr = (uint32_t)&LPC_ADC->ADDR0;
    lli.dstAddr = (uint32_t)&LPC_DAC->DACR;
    lli.nextLLI = (uint32_t)&lli; // la apunto a si misma, luego de inicializarla
    lli.control =
        (4095 | 1 << 18 | 1 << 21); // 4095 datos, dato halfword en source(18) y destination (21)

    // Call the configuration functions for each peripheral
    UART_Config();
    TECLADO_Config();
    TIMER_PWM_Config();
    servo_GPIO();
    I2C_Config();
    ADC_Config();
    DMA_Config(&lli);

    while (1) {
        get_ADC_Value(); // Read the ADC value and update the servo position accordingly
        choose_Action(); // Check if a button was pressed and perform the corresponding action
    }

    return 0;
}
