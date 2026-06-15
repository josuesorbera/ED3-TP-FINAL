#include <stdio.h>
#include "I2C.h"

static system_state_t current_state = SYSTEM_OFF;

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

void LCD_Set_Cursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? 0x80 : 0xC0;
    LCD_Send_Byte(address + col, 0);
}

// Function to update LCD based on system state
void LCD_Update_Status(system_state_t state) {
    current_state = state;
    LCD_Set_Cursor(0, 0);
    LCD_Send_String("                "); // Limpiar línea

    LCD_Set_Cursor(0, 0);

    switch(state) {
        case SYSTEM_ON:
            LCD_Send_String("VIGILANCIA ON");
            LCD_Set_Cursor(1, 0);
            LCD_Send_String("                "); // Limpiar línea
            LCD_Set_Cursor(1, 0);
            LCD_Send_String("ADC: ACTIVO");
        break;

        case SYSTEM_OFF:
            LCD_Send_String("SISTEMA APAGADO");
            LCD_Set_Cursor(1, 0);
            LCD_Send_String("                "); // Limpiar línea
            LCD_Set_Cursor(1, 0);
            LCD_Send_String("Sleep Mode");
        break;

        case POSITION_SET:
            LCD_Send_String("POSICION FIJADA");
            LCD_Set_Cursor(1, 0);
            LCD_Send_String("                "); // Limpiar línea
            LCD_Set_Cursor(1, 0);
            LCD_Send_String("ADC: APAGADO");
        break;
    }
}

// Function to get the current system state
system_state_t LCD_Get_Current_State(void) {
    return current_state;
}