#include <stdio.h>
#include "I2C.h"

// Initialize function for I2C
void I2C_Config(void){
    I2C_Init(LPC_I2C1, 100000); // Inicialize I2C1 a 100kHz
    I2C_Cmd(LPC_I2C1, ENABLE);  // Enable I2C1
}

// Send byte to the PCF8574
void LCD_Send_Byte(uint8_t data, uint8_t mode){
    I2C_M_SETUP_Type txsetup = {0}; // Initialize the structure to zero
    uint8_t byte_to_send = data;

    if(mode == 0) { // Command
        byte_to_send &= ~RS_PIN; // RS = 0
    } else { // Data
        byte_to_send |= RS_PIN;  // RS = 1
    }

    txsetup.sl_addr7bit = LCD_ADDRESS >> 1; // Shift address to 7 bits
    txsetup.tx_data = &byte_to_send; // Pointer to the byte to send
    txsetup.tx_length = 1; // We are sending one byte
    txsetup.rx_data = NULL; // No data to receive
    txsetup.retransmissions_max = 2; // Max retransmissions

    I2C_MasterTransferData(LPC_I2C1, &txsetup, I2C_TRANSFER_POLLING);
}

// Send string to the LCD
void LCD_Send_String(char* str){
    while(*str){
        LCD_Send_Byte(*str, 1); // data mode
        str++;
    }
}

// Show angle of the servo on the LCD
void LCD_Display_Servo_Angle(float angle){
    char buffer[20];
    sprintf(buffer, "Angle: %.1f", angle);
    LCD_Send_String(buffer);
}