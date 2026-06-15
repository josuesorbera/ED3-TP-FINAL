#include <stdio.h>
#include "I2C.h"

static system_state_t current_state = SYSTEM_OFF;

// Initialize function for I2C
void I2C_Config(void){
    PINSEL_CFG_T pinCfg = {0}; // Initialize the structure to zero

    pinCfg.port = PORT_0;
    pinCfg.pin = PIN_0;
    pinCfg.func = PINSEL_FUNC_11;     // SDA1
    pinCfg.mode = PINSEL_TRISTATE;
    pinCfg.openDrain = ENABLE;

    PINSEL_ConfigPin(&pinCfg);

    pinCfg.pin = PIN_1;               // SCL1

    PINSEL_ConfigPin(&pinCfg);

    I2C_Init(LPC_I2C1, 100000); // Inicialize I2C1 a 100kHz
    I2C_Cmd(LPC_I2C1, ENABLE);  // Enable I2C1
}

void I2C_Write(uint8_t data){
    I2C_M_SETUP_Type txsetup = {0}; // Initialize the structure to zero

    txsetup.sl_addr7bit = LCD_ADDRESS >> 1; // Shift address to 7 bits
    txsetup.tx_data = &data; // Pointer to the data to send
    txsetup.tx_length = 1; // We are sending one byte
    txsetup.rx_data = NULL; // No data to receive
    txsetup.retransmissions_max = 2; // Max retransmissions

    I2C_MasterTransferData(LPC_I2C1, &txsetup, I2C_TRANSFER_POLLING);
}

// Send byte to the PCF8574
void LCD_Send_Byte(uint8_t data, uint8_t mode){
    uint8_t high_nibble = data & 0xF0;
    uint8_t low_nibble = (data << 4) & 0xF0;
    uint8_t control_bits = BL_PIN;

    if(mode)
    {
        control_bits |= RS_PIN;
    }

    // High nibble
    I2C_Write(high_nibble | control_bits | EN_PIN);
    for(volatile uint32_t i=0;i<2000;i++);
    I2C_Write(high_nibble | control_bits); // Falling edge (Capture)
    for(volatile uint32_t i=0;i<2000;i++);

    // Low nibble
    I2C_Write(low_nibble | control_bits | EN_PIN);
    for(volatile uint32_t i=0;i<2000;i++);
    I2C_Write(low_nibble | control_bits); // Falling edge (Capture)
    for(volatile uint32_t i=0;i<5000;i++);

}

// Send string to the LCD
void LCD_Send_String(char *str){
    while(*str){
        LCD_Send_Byte(*str, 1); // data mode
        str++;
    }
}

void LCD_Set_Cursor(uint8_t row, uint8_t col) {
    uint8_t address;

    if(row == 0)
    {
        address = 0x80;
    }
    else
    {
        address = 0xC0;
    }

    LCD_Send_Byte(address + col,0);
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