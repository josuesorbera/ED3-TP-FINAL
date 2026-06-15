#ifndef TECLADO_H
#define TECLADO_H

// Libraries inclusion
#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"
#include "ADC.h"
#include "TIMER_PWM.h"

// Macro definitions
extern volatile uint8_t tecla_presionada;
extern volatile uint8_t flag_teclado;

// Function prototypes
void TECLADO_Config(void);
void EINT3_IRQHandler(void);
void SysTick_Handler(void);
void choose_Action(void);

#endif// TECLADO_H
