#include <stdio.h>
#include "ADC.h"

void ADC_Config(void) {
    ADC_Init(200000); // ADC clock is 200KHz
    ADC_PinConfig(ADC_CHANNEL_0); // Configure pin for channel 0
    ADC_StartCmd(ADC_START_CONTINUOUS); // Start continuous conversion
    ADC_ChannelEnable(ADC_CHANNEL_0); // Enable channel 0
    ADC_BurstEnable(ENABLE); // Enable burst mode
    ADC_PowerDown(); //Inicia apagado, lo encendemos con un boton del Teclado
}