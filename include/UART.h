#ifndef UART_H
#define UART_H

// Libraries inclusion
#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_uart.h"

// Macro definitions


// Function prototypes
void UART_Config(void);
static void uart_send_data(uint16_t adc_val, uint32_t pulse_us)
static uint8_t uint32_to_str(uint32_t val, uint8_t *buf)

#endif // UART_H
