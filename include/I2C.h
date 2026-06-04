#ifndef I2C_H
#define I2C_H

// Libraries inclusion
#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_i2c.h"

// Macro definitions
#define LCD_ADDRESS 0x4E // Typical address

// Function prototypes
void DMA_Config(void);
void LCD_Send_Byte(uint8_t data, uint8_t mode);
void LCD_Send_String(char* str);

#endif // I2C_H
