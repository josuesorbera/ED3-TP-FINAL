#include <stdio.h>
#include "DMA.h"

void DMA_Config(GPDMA_LLI_T* lli) {
    GPDMA_Init();

    GPDMA_Endpoint_T endCfg = {0};
    endCfg.width = GPDMA_HALFWORD;
    endCfg.burst = GPDMA_BSIZE_1;
    endCfg.increment = DISABLE;

    GPDMA_Channel_CFG_T configDMA = {0};
    configDMA.channelNum = GPDMA_CH_0;
    configDMA.transferSize = 4095; // para hacer la maxima cantidad de transferencias y luego reconfigurar
    configDMA.type = GPDMA_P2M; // tratamos al DAC como memoria, ya que aguanta la frecuencia de muestreo
    // de 200kHz del ADC y queremos la mayor velocidad de transferencia
    configDMA.srcMemAddr = (uint32_t)&LPC_ADC->ADDR0; // Lee del registro del Canal 0 o el global
    configDMA.dstMemAddr = (uint32_t)&LPC_DAC->DACR;  // Escribe en el registro del DAC
    configDMA.srcConn = GPDMA_ADC;
    // configDMA.dstConn = 0; el destino es memoria por lo que queda en 0
    configDMA.src = endCfg;
    configDMA.dst = endCfg;
    configDMA.intTC = DISABLE;
    configDMA.intErr = DISABLE;
    configDMA.linkedList = (uint32_t)lli; // sin ampersand

    GPDMA_SetupChannel(&configDMA);
    GPDMA_ChannelStart(GPDMA_CH_0);

    // Falta el otro canal del GPDMA para transferencia ADC a UART
}