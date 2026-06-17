#include "LED.h"

void LED_Config(void) {
    PINSEL_CFG_T pinCfg = {0};
    pinCfg.port = PORT_0;
    pinCfg.pin = PIN_4; // P0.22
    pinCfg.func = PINSEL_FUNC_00; // GPIO
    pinCfg.mode = PINSEL_TRISTATE;
    pinCfg.openDrain = DISABLE;
    PINSEL_ConfigPin(&pinCfg);

    GPIO_SetDir(PORT_0, (1 << 4), GPIO_OUTPUT); // Set the pin (P0.22) as output
    GPIO_ClearPins(PORT_0, (1 << 4)); // Start with the LED off
}