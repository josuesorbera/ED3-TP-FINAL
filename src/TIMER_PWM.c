#include <stdio.h>
#include "TIMER_PWM.h"

void TIMER_PWM_Config(void) {
     TIMER_CFG_T timCfg = {0}; // Preliminar values for the timer
     timCfg.prescaleOpt = TIM_US;
     timCfg.prescaleValue = 1; // 1us tick

     TIM_InitTimer(LPC_TIM0, &timCfg);

     // Match 0: Period of 20ms (50Hz)
     TIM_MATCHCFG_T match0Cfg = {0}; // Preliminar values for the match channel 0
     match0Cfg.channel = TIM_MATCH_0;
     match0Cfg.intEn = ENABLE;
     match0Cfg.stopEn = DISABLE;
     match0Cfg.resetEn = ENABLE; // Reset when gets to 20ms
     match0Cfg.extOpt = TIM_NOTHING;
     match0Cfg.matchValue = 20000; // 20ms

     // Match 1: Pulse width (1-2ms)
     TIM_MATCHCFG_T match1Cfg = {0}; // Preliminar values for the match channel 1
     match1Cfg.channel = TIM_MATCH_1;
     match1Cfg.intEn = ENABLE;
     match1Cfg.stopEn = DISABLE;
     match1Cfg.resetEn = DISABLE;
     match1Cfg.extOpt = TIM_NOTHING;
     match1Cfg.matchValue = 1500; // 1.5ms (neutral position for the servo)

     TIM_ConfigMatch(LPC_TIM0, &match0Cfg) // The PWM implements two match channels, one for the period and another for the duty cycle,
     TIM_ConfigMatch(LPC_TIM0, &match1Cfg) // one with reset and the other without, so the timer counter is reset on match0 and match1 is used to control the duty cycle.

     NVIC_EnableIRQ(TIMER0_IRQn);
     TIM_Enable(LPC_TIM0);
}

void servo_GPIO(void) {
     GPIO_SetDir(PORT_0, (1 << 10), GPIO_OUTPUT); // Set the pin (P0.10) as output
}

// Function to give the position to the servo (0-180 grados)
void servo_Position(uint8_t angle) {
     if (angle > 180) angle = 180; // Limit the angle to 180 degrees
     uint32_t pulseWidth = 1000 + ((uint32_t)angle * 1000 / 180);

     TIM_UpdateMatchValue(LPC_TIM0, TIM_MATCH_1, pulseWidth);
}

void TIMER0_IRQHandler(void) {
     if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)) {
          // Begin of the period
          GPIO_SetPinState(PORT_0, (1 << 10), SET);  // Pulse in HIGH on MR0
          TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
     }

     if (TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT)) {
          // End of the pulse
          GPIO_SetPinState(PORT_0, (1 << 10), 0);  // Pulse in LOW on MR1
          TIM_ClearIntPending(LPC_TIM0, TIM_MR1_INT);
     }
}