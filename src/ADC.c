#include <stdio.h>
#include "ADC.h"

void ADC_Config(void) {
    ADC_Init(200000);                 // ADC clock is 200KHz
    ADC_PinConfig(ADC_CHANNEL_0);     // Configure pin for channel 0
    ADC_ChannelEnable(ADC_CHANNEL_0); // Enable channel 0
    // ADC initiates turned off, it can be turned on by pressing a button
}
