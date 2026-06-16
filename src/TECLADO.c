#include <stdio.h>
#include "TECLADO.h"

// Global volatile variables for the flags
volatile uint8_t tecla_presionada = 0xFF;
volatile uint8_t flag_teclado = 0;

void TECLADO_Config(void) {
    SYSTICK_InternalInit(20); // Configure SysTick to generate an interrupt every 20ms to read the keyboard

    PINSEL_CFG_T configPin = {0};     // Struct to configure the pins, initialized to zero
    // Configure rows P2.0 and P2.1
    configPin.port = PORT_2;
    configPin.func = PINSEL_FUNC_00;
    configPin.openDrain = DISABLE;
    configPin.mode = PINSEL_TRISTATE; // Tristate for outputs

    PINSEL_ConfigMultiplePins(&configPin, 3);
    GPIO_SetDir(PORT_2, 3, GPIO_OUTPUT); // Rows as outputs

    GPIO_ClearPins(PORT_2, 3); // Rows on 0V to generate a falling edge when the button is pressed

    // Configure columns P2.2 and P2.3
    configPin.mode = PINSEL_PULLUP; // Pullups for inputs

    PINSEL_ConfigMultiplePins(&configPin, (3 << 2));
    GPIO_SetDir(PORT_2, (3 << 2), GPIO_INPUT); // Columns as inputs

    GPIO_IntConfigPort(PORT_2, (3 << 2), GPIO_INT_FALLING);
    NVIC_EnableIRQ(EINT3_IRQn);      // Enable interruption for falling edge in P2.2 and P2.3
    NVIC_SetPriority(EINT3_IRQn, 1); // Set priority
}

// Handler to detect the button pressed
void EINT3_IRQHandler(void) {
    NVIC_DisableIRQ(EINT3_IRQn);     // Disable interruptions to avoid noise while waiting for the Sysyick
    GPIO_ClearInt(PORT_2, (1 << 2)); // Set flag for column 1
    GPIO_ClearInt(PORT_2, (1 << 3)); // Set flag for column 2
    NVIC_ClearPendingIRQ(EINT3_IRQn);

    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE); // Enable Systick to count 20 ms(delay antidebounce))
}

void SysTick_Handler(void) {
    static uint8_t esperando_liberacion = 0; // Variable to track when no button is being pressed

    uint8_t estado_actual = 0xFF; // Initialize variable in 0xFF to avoid wrong lectures

    // Scan row 1
    GPIO_SetPins(PORT_2, (1 << 0));   // Row 1 at 3.3V
    GPIO_ClearPins(PORT_2, (1 << 1)); // Row 2 at 0V

    uint32_t puerto = GPIO_ReadValue(PORT_2);
    if ((puerto & (1 << 2)) == 0) estado_actual = 1;      // R1-C1
    else if ((puerto & (1 << 3)) == 0) estado_actual = 3; // R1-C2

    // Scan row 2
    GPIO_ClearPins(PORT_2, (1 << 0)); // Row 1 at 0V
    GPIO_SetPins(PORT_2, (1 << 1));   // Row 2 at 3.3V

    puerto = GPIO_ReadValue(PORT_2);
    if ((puerto & (1 << 2)) == 0) estado_actual = 2;      // R2-C1
    else if ((puerto & (1 << 3)) == 0) estado_actual = 4; // R2-C2


    // Block logic for pulse detection and release detection
    if (estado_actual != 0xFF) { // If the button was pressed, it entrys the if
        if (esperando_liberacion == 0) {
            // First touch, register data
            tecla_presionada = estado_actual;
            flag_teclado = 1;
            esperando_liberacion = 1; // "Locks" while is being pressed
        }

        GPIO_ClearPins(PORT_2, 3); // Put rows at 0V to avoid multiple detections
        // Systick continues running and entering here every 20ms, until the button is released
    } else {
        // The button was released, clean flag and prepare for the next press
        esperando_liberacion = 0;

        SYSTICK_Cmd(DISABLE);
        SYSTICK_IntCmd(DISABLE); // Disable Systick

        GPIO_ClearPins(PORT_2, 3); // Put rows at 0V to avoid multiple detections

        // Clean interrupt flags in port 2
        GPIO_ClearInt(PORT_2, (1 << 2));
        GPIO_ClearInt(PORT_2, (1 << 3));
        NVIC_ClearPendingIRQ(EINT3_IRQn);

        // Enable interruptions again for the next button press
        NVIC_EnableIRQ(EINT3_IRQn);
    }
}

void choose_Action(void) {
    if (flag_teclado == 1) {
        switch (tecla_presionada) {
            case 1:
                LCD_Update_Status(SYSTEM_ON); // Update the LCD to system on mode
				ADC_PowerUp();
                ADC_BurstEnable(); // Turn on ADC
                GPDMA_ChannelStart(GPDMA_CH_0); //Enable Channel 0 GPDMA
                break;
            case 2:
                LCD_Update_Status(POSITION_SET); // Update the LCD to position set mode
                ADC_BurstDisable(); // Turn off ADC
                break;
            case 3:
                //Mensaje de Advertencia
                LCD_Update_Status(SYSTEM_OFF); // Update the LCD to system off mode
                GPIO_TogglePins(PORT_0, (1 << 4)); //P0.4 -> H39
                break;
        }
    }

    flag_teclado = 0; // Put the flag to 0
}