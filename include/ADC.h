#ifndef ADC_H
#define ADC_H

// Libraries inclusion
#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_adc.h"
#include "TIMER_PWM.h"

// Macro definitions


// Function prototypes
void ADC_Config(void);
void get_ADC_Value(void);

#endif // ADC_H