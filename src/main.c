#include <stdio.h>
#include <stdlib.h>
#include "LPC17xx.h"
#include "../inc/ADC.h"
#include "../inc/UART.h"
#include "../inc/TECLADO.h"
#include "../inc/I2C.h"
#include "../inc/DMA.h"
#include "../inc/LED.h"
#include "../inc/TIMER_PWM.h"
#include "../inc/TIMER_UART.h"

int main(void) {
    // Configuramos la estructura LLI circular apuntando a sí misma
    lli.srcAddr = (uint32_t)&(LPC_ADC->ADGDR);
    lli.dstAddr = (uint32_t)adc_buffer;
    lli.nextLLI = (uint32_t)&lli;
    // Control LLI: 64 muestras | Origen 32 bits | Destino 32 bits | Auto-incremento en RAM | Int
    // Enable
    lli.control = (SAMPLES | (2 << 18) | (2 << 21) | (1 << 27) | (1UL << 31));
    // Call the configuration functions for each peripheral
    UART_Config();
    TECLADO_Config();
    servo_GPIO();
    TIMER_PWM_Config();
    TIMER_UART_Config();
    LED_Config();
    I2C_Config();
    LCD_Init();
    ADC_Config();
    DMA_Config();

    while (1) {
        choose_Action();
        if (flag_imprimir == 1) {
            uart_send_data(adcValue, pulseWidth);
            flag_imprimir = 0;
        }
    }
    return 0;
}