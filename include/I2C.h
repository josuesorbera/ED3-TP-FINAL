#ifndef I2C_H
#define I2C_H

// Libraries inclusion
#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"

// Macro definitions (address and control bits for PCF8574)
#define LCD_ADDRESS (0x4E) // Typical address for PCF8574 (0x27 shifted left by 1 bit)
#define RS_PIN 0x01 // 0=Command, 1=Data (1 shifted left by 0 bits)
#define RW_PIN 0x02  // Read/Write (always in 0 for writing to LCD) (1 shifted left by 1 bit)
#define EN_PIN 0x04  // Enable pulse (1 shifted left by 2 bits)
#define BL_PIN 0x08  // 1=Backlight on (1 shifted left by 3 bits)
typedef enum {
    SYSTEM_ON,
    SYSTEM_OFF,
    POSITION_SET
} system_state_t; // Define an enum for system states

// Function prototypes
void I2C_Config(void);
void I2C_Write(uint8_t data);
void LCD_Init(void);
void LCD_Send_Byte(uint8_t data, uint8_t mode);
void LCD_Send_String(char *str);
void LCD_Set_Cursor(uint8_t row, uint8_t col);
void LCD_Update_Status(system_state_t state);
system_state_t LCD_Get_Current_State(void);

#endif // I2C_H
