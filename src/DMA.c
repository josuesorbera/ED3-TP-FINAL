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
    configDMA.transferSize = 4095;      // Max transfer size for 12-bit ADC
    configDMA.type = GPDMA_P2M;         // To send to a memory address
    configDMA.dstMemAddr = DIR_VALADC;  // Writes the ADC value to this address
    configDMA.srcConn = GPDMA_ADC;
    configDMA.src = endCfg;
    configDMA.dst = endCfg;
    configDMA.intTC = DISABLE;
    configDMA.intErr = DISABLE;
    configDMA.linkedList = (uint32_t)lli;

    GPDMA_SetupChannel(&configDMA);
    GPDMA_ChannelStart(GPDMA_CH_0);

}