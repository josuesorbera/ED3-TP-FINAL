#include <stdio.h>
#include "TIMER_PWM.h"
#include "DMA.h"

void TIMER_PWM_Config(void) {
    TIM_TIMERCFG_T timCfg = {0};     // Preliminar values for the timer
    timCfg.prescaleOpt = TIM_US;
    timCfg.prescaleValue = 1;     // 1us tick

    TIM_InitTimer(LPC_TIM0, &timCfg);

    // Match 0: Period of 20ms (50Hz)
    TIM_MATCHCFG_T match0Cfg = {0}; // Preliminar values for the match channel 0
    match0Cfg.channel = TIM_MATCH_0;
    match0Cfg.intEn = ENABLE;
    match0Cfg.stopEn = DISABLE;
    match0Cfg.resetEn = ENABLE;     // Reset when gets to 20ms
    match0Cfg.extOpt = TIM_NOTHING;
    match0Cfg.matchValue = 20000;   // 20ms

    // Match 1: Pulse width (1-2ms)
    TIM_MATCHCFG_T match1Cfg = {0}; // Preliminar values for the match channel 1
    match1Cfg.channel = TIM_MATCH_1;
    match1Cfg.intEn = ENABLE;
    match1Cfg.stopEn = DISABLE;
    match1Cfg.resetEn = DISABLE;
    match1Cfg.extOpt = TIM_NOTHING;
    match1Cfg.matchValue = 1500;   // 1.5ms (neutral position for the servo)

    TIM_ConfigMatch(LPC_TIM0, &match0Cfg); // The PWM implements two match channels, one for the
                                           // period and another for the duty cycle,
    TIM_ConfigMatch(LPC_TIM0, &match1Cfg); // one with reset and the other without, so the timer counter is
                                           // reset on match0 and match1 is used to control the duty cycle.

    NVIC_EnableIRQ(TIMER0_IRQn);
    NVIC_SetPriority(TIMER0_IRQn, 0); // Set a priority for the timer interrupt
    TIM_Enable(LPC_TIM0);
}

void servo_GPIO(void) {

    PINSEL_CFG_T pinCfg = {0};
    pinCfg.port = PORT_0;
    pinCfg.pin = PIN_10;
    pinCfg.func = PINSEL_FUNC_00; // Set function to GPIO
    pinCfg.mode = PINSEL_TRISTATE;
    pinCfg.openDrain = DISABLE;
    PINSEL_ConfigPin(&pinCfg);

    GPIO_SetDir(PORT_0, (1 << 10), GPIO_OUTPUT); // Set the pin (P0.10) as output
    GPIO_ClearPins(PORT_0, (1 << 10));
}

// Function to move the servo based on joystick ADC value (0-4095)
void servo_Position(uint16_t nuevoAdc) {
    static uint32_t adc_suavizado = 2045;
    static uint16_t ultimo_adc_valido = 2045;

    // filtros para suavizar la señal
    adc_suavizado = ((adc_suavizado * 3) + nuevoAdc) / 4;
    uint16_t adc_filtrado = (uint16_t)adc_suavizado;

    if (abs(adc_filtrado - ultimo_adc_valido) > 50) {
        ultimo_adc_valido = adc_filtrado;
    }
    uint16_t valor_final = ultimo_adc_valido;

    //rangos establecidos para zona muerta y límite del ADC
    if (valor_final > 3900) valor_final = 4095;
    if (valor_final < 300) valor_final = 0;
    if (valor_final < 2200 && valor_final > 2100) valor_final = 2045;

    pulseWidth = 500 + (((uint32_t)valor_final * 2000) / 4095);
}

void TIMER0_IRQHandler(void) {
    if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)) {
        // Begin of the period
        GPIO_SetPins(PORT_0, (1 << 10)); // Pulse in HIGH on MR0
        TIM_UpdateMatchValue(LPC_TIM0, TIM_MATCH_1, pulseWidth);
        TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
    }

    if (TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT)) {
        // End of the pulse
        GPIO_ClearPins(PORT_0, (1 << 10)); // Pulse in LOW on MR1

        TIM_ClearIntPending(LPC_TIM0, TIM_MR1_INT);
    }
}