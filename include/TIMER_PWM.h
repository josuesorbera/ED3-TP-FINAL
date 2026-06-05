#ifndef TIMER_PWM_H
#define TIMER_PWM_H

// Libraries inclusion
#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"

// Macro definitions


// Function prototypes
void TIMER_PWM_Config(void);
void servo_GPIO(void);
void servo_Position(uint8_t angle);

#endif // TIMER_PWM_H
