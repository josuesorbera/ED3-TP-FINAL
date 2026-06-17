#ifndef DMA_H
#define DMA_H

// Libraries inclusion
#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_gpdma.h"
#include "TIMER_PWM.h"

// Macro definitions
extern GPDMA_LLI_T lli;
#define DIR_VALADC 0x2007C000
#define SAMPLES 64
extern volatile uint32_t adc_buffer[SAMPLES];
extern volatile uint16_t adcValue;
extern volatile uint32_t pulseWidth;

// Function prototypes
void DMA_Config(void);

#endif // DMA_H
