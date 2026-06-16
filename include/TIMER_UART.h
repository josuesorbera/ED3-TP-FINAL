#ifndef TIMER_UART_H
#define TIMER_UART_H

// Libraries inclusion
#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_timer.h"

// Macro definitions
extern volatile uint8_t flag_imprimir;

// Function prototypes
void TIMER_UART_Config(void);
void TIMER1_IRQHandler(void);

#endif //TIMER_UART_H
