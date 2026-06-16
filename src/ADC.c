#include <stdio.h>
#include "ADC.h"

void ADC_Config(void) {
    ADC_Init(200000);                   // ADC clock is 200KHz
    ADC_PinConfig(ADC_CHANNEL_0);       // Configure pin for channel 0
    ADC_ChannelEnable(ADC_CHANNEL_0);   // Enable channel 0
    ADC_StartCmd(ADC_START_CONTINUOUS); // Start continuous conversion
    ADC_BurstEnable();                  // Enable burst mode
    ADC_PowerDown();                    // Iniciates turned off so we can can start it with a button
}

uint16_t get_ADC_Value(void) {
    /*
    uint16_t adcValue;

    if (ADC_ChannelGetStatus(ADC_CHANNEL_0, ADC_DATA_DONE)) {
        adcValue = ADC_ChannelGetData(ADC_CHANNEL_0);

        servo_Position(adcValue);
    }
    */
    uint16_t* ptrADC = (uint16_t*)DIRVALADC;
    return *ptrADC & 0xFFF;  // Mask to get the 12-bit ADC value
}
