#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_timer.h"

void TIMER_PWM_Config(void) {
     TIMER_CFG_T timCfg = {0} // Preliminar values for the timer
     timCfg.prescaleOpt = TIM_US;
     timCfg.prescaleValue = 1000; // 1ms tick

     TIM_InitTimer(LPC_TIM0, &timCfg);

     TIM_MATCHCFG_T match0Cfg = {0}; // Preliminar values for the match channel 0
     match0Cfg.channel = TIM_MATCH_0;
     match0Cfg.intEn = DISABLE;
     match0Cfg.stopEn = DISABLE;
     match0Cfg.resetEn = ENABLE;
     match0Cfg.extOpt = TIM_NOTHING;
     match0Cfg.matchValue = 1000;

     TIM_MATCHCFG_T match1Cfg = {0}; // Preliminar values for the match channel 1
     match1Cfg.channel = TIM_MATCH_1;
     match1Cfg.intEn = DISABLE;
     match1Cfg.stopEn = DISABLE;
     match1Cfg.resetEn = DISABLE;
     match1Cfg.extOpt = TIM_NOTHING;
     match1Cfg.matchValue = 1000;

     TIM_ConfigMatch(LPC_TIM0, &match0Cfg) // The PWM implements two match channels, one for the period and another for the duty cycle,
     TIM_ConfigMatch(LPC_TIM0, &match1Cfg) // one with reset and the other without, so the timer counter is reset on match0 and match1 is used to control the duty cycle.

     TIM_Enable(LPC_TIM0);
}