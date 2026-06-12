#include <stdio.h>
#include "ADC.h"

void ADC_Config(void) {
    ADC_Init(100000); // ADC clock is 200KHz
    ADC_PinConfig(ADC_CHANNEL_0); // Configure pin for channel 0
    ADC_StartCmd(ADC_START_CONTINUOUS); // Start continuous conversion
    ADC_ChannelEnable(ADC_CHANNEL_0); // Enable channel 0
    ADC_BurstEnable(ENABLE); // Enable burst mode
    ADC_PowerDown(); //Inicia apagado, lo encendemos con un boton del Teclado
}

static uint32_t adc_read_blocking(void){
    ADC_StartCmd(ADC_START_NOW);
    while (ADC_ChannelGetStatus(ADC_CHANNEL_0, ADC_DATA_DONE) == RESET);
    return (uint32_t)ADC_ChannelGetData(ADC_CHANNEL_0);
}

static uint32_t map_adc_to_pulse(uint32_t adc_val){
    return PULSE_MIN_US + (adc_val * (PULSE_MAX_US - PULSE_MIN_US)) / ADC_MAX;
}
