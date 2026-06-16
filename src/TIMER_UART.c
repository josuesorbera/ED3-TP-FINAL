#include <stdio.h>
#include "TIMER_UART.h"

volatile uint8_t flag_imprimir = 0;

void TIMER_UART_Config(void) {
    TIM_TIMERCFG_T timCfg = {0};
    timCfg.prescaleOpt = TIM_US;
    timCfg.prescaleValue = 1; // 1us tick

    TIM_InitTimer(LPC_TIM1, &timCfg);

    // Match 0: Período de 2 segundos
    TIM_MATCHCFG_T match0Cfg = {0};
    match0Cfg.channel = TIM_MATCH_0;
    match0Cfg.intEn = ENABLE;
    match0Cfg.stopEn = DISABLE;
    match0Cfg.resetEn = ENABLE; // Reset when it reaches the limit
    match0Cfg.extOpt = TIM_NOTHING;
    match0Cfg.matchValue = 2000000; // 2,000,000 us = 2s

    TIM_ConfigMatch(LPC_TIM1, &match0Cfg);

    NVIC_EnableIRQ(TIMER1_IRQn);
    NVIC_SetPriority(TIMER1_IRQn, 2); // Low priority to not interfere with the servo control and button reading
    TIM_Enable(LPC_TIM1);
}

void TIMER1_IRQHandler(void) {
    if (TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT)) {

        flag_imprimir = 1; // Inform main that 2 seconds have passed

        TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);
    }
}