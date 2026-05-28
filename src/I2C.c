#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_i2c.h"

#define LCD_ADDRESS 0x4E // Dirección típica del LCD I2C (ajusta según tu dispositivo)

// Configuration that needs to be checked before using

void I2C_Config(void){
    I2C_Init(LPC_I2C1, 100000); // Inicializa I2C1 a 100kHz
    I2C_Cmd(LPC_I2C1, ENABLE);  // Habilita I2C1
}

void LCD_Send_Byte(uint8_t data, uint8_t mode){
    I2C_M_SETUP_Type txsetup;

    txsetup.sl_addr7bit = LCD_ADDRESS >> 1;
    txsetup.tx_data = &data;
    txsetup.tx_length = 1;
    txsetup.rx_data = NULL;
    txsetup.rx_length = 0;
    txsetup.retransmissions_max = 2;

    I2C_MasterTransferData(LPC_I2C1, &txsetup, I2C_TRANSFER_POLLING);
}

void LCD_Send_String(char* str){
    while(*str){
        LCD_Send_Byte(*str, 1); // modo datos
        str++;
    }
}