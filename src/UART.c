#include <stdio.h>
#include "UART.h"

void UART_Config(void) {

    UART_CFG_T UARTcfg = {0}; // Initialize structure in 0
    UARTcfg.baudRate = 9600;
    UARTcfg.parity = UART_PARITY_NONE;
    UARTcfg.dataBits = UART_DBITS_8;
    UARTcfg.stopBits = UART_STOPBIT_1;

    UART_FIFO_CFG_T fifo_cfg = {.resetRxBuf = ENABLE, .resetTxBuf = ENABLE, .dmaMode = DISABLE, .level = UART_FIFO_TRGLEV0};

    UART_PinConfig(UART_TX0_P0_2);
    UART_PinConfig(UART_RX0_P0_3);
    UART_Init(UART0, &UARTcfg);        // Initialize UART0 with the specified configuration
    UART_FIFOConfig(UART0, &fifo_cfg); // Configure UART0 FIFO settings
    UART_TxEnable(UART0);
}

static void uart_send_data(uint16_t adc_val, uint32_t pulse_us) {
    uint8_t buf[32];
    uint8_t idx = 0;

    buf[idx++] = 'A';
    buf[idx++] = 'D';
    buf[idx++] = 'C';
    buf[idx++] = ':';
    idx += uint32_to_str((uint32_t)adc_val, &buf[idx]);
    buf[idx++] = ',';
    buf[idx++] = ' ';
    buf[idx++] = 'P';
    buf[idx++] = 'W';
    buf[idx++] = 'M';
    buf[idx++] = ':';
    idx += uint32_to_str(pulse_us, &buf[idx]);
    buf[idx++] = '\r';
    buf[idx++] = '\n';
    buf[idx++] = '\n';

    UART_Send(UART0, buf, idx, BLOCKING);
}

static uint8_t uint32_to_str(uint32_t val, uint8_t* buf) {
    uint8_t digits[10];
    uint8_t n = 0;
    if (val == 0) {
        buf[0] = '0';
        return 1;
    }

    while (val > 0) {
        digits[n++] = (uint8_t)('0' + val % 10);
        val /= 10;
    }

    for (uint8_t i = 0; i < n; i++)

        buf[i] = digits[n - 1 - i];
    return n;
}
