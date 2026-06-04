#include <stdio.h>
#include "UART.h"

void UART_Config(void) {

    // NEEDS COMPLETITION OF THE STRUCTURE WITH THE PROPER VALUES

    UART_CFG_T uartCfg = {0}; // Preliminar values for the UART configuration
    uartCfg.baudRate = 115200; // Set baud rate to 115200 bps
    uartCfg.dataBits = UART_DATABIT_8; // 8 data bits
    uartCfg.stopBits = UART_STOPBIT_1; // 1 stop bit
    uartCfg.parity = UART_PARITY_NONE; // No parity

    UART_Init(UART0, &uartCfg); // Initialize UART0 with the specified configuration
}