#include <stdio.h>
#include "DAC.h"

void DAC_Config(void){
    DAC_Init(); // Inicialize DAC (pin and clock)
    DAC_SetBias(DAC_350uA); // 350uA alows to take samples up to 400kHz, 700uA alows up to 1MHz
} // It's no necesary to use ConfigDAConverterControl (we use DAC as a memory address not as a periferic)