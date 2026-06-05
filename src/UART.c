#include <stdio.h>
#include "UART.h"

void UART_Config(void) {

    // NEEDS COMPLETITION OF THE STRUCTURE WITH THE PROPER VALUES

    UART_CFG_T UARTcfg ={0}; //le pongo valores por defecto
    UARTcfg.baudRate = 9600;
    UARTcfg.parity = UART_PARITY_NONE;
    UARTcfg.dataBits = UART_DBITS_8;
    UARTcfg.stopBits = UART_STOPBIT_1;

    UART_Init(UART0, &uartCfg); // Initialize UART0 with the specified configuration
}