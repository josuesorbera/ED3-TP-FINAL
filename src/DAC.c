#include <stdio.h>
#include "DAC.h"

void cfgDAC(void){
    DAC_Init(); // inicializa el DAC (pin y por lo tanto, clock)
    DAC_SetBias(DAC_350uA); // 350uA permite tomar muestras hasta 400k, 700uA permite hasta 1MHz
} //no es necesario usar ConfigDAConverterControl ya que se trata DAC como memoria y no como periferico