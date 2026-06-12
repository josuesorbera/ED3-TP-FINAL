#ifndef TECLADO_H
#define TECLADO_H

//librerias
#include "LPC17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"

//variables en TECLADO.c
extern volatile uint8_t tecla_presionada;
extern volatile uint8_t flag_teclado;


//prototipos de funcion
void TECLADO_Config(void);

#endif
