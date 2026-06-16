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
    ADC_Config();
    DMA_Config(&lli);

    while (1) {
        choose_Action(); // Check if a button was pressed and perform the corresponding action

        uint16_t adcValue = get_ADC_Value(); // Read the ADC value and update the servo position accordingly
        servo_Position(adcValue);

        if(flag_imprimir){
            uart_send_data(ADC_ChannelGetData(ADC_CHANNEL_0), pulseWidth); // If it doesn't work, use ADC_GlobalGetData()
            flag_imprimir = 0;
        }
    }

    return 0;
}
