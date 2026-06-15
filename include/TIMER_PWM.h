#ifndef TIMER_PWM_H
#define TIMER_PWM_H

// Libraries inclusion
#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

// global variables 
extern volatile uint32_t pulseWidth;

// Function prototypes
void TIMER_PWM_Config(void);
void servo_GPIO(void);
void servo_Position(uint16_t adcValue);
void TIMER0_IRQHandler(void);

#endif // TIMER_PWM_H
