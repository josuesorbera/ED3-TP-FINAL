#ifndef ADC_H
#define ADC_H

// Libraries inclusion
#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_adc.h"

// Macro definitions


// Function prototypes
void ADC_Config(void);
static uint32_t adc_read_blocking(void)
static uint32_t map_adc_to_pulse(uint32_t adc_val)
#endif // ADC_H