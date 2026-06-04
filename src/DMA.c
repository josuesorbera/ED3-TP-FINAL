#include <stdio.h>
#include "DMA.h"

void DMA_Config(void){
    GPDMA_Channel_CFG_T dmaCfg = {0}; // Preliminar values for the DMA channel configuration

    // NEEDS COMPLETITION OF THE STRUCTURE WITH THE PROPER VALUES

    GPDMA_Init(); // Initialize the GPDMA controller
    GPDMA_SetupChannel(&dmaCfg);
    GPDMA_ChannelStart(GPDMA_CH_0); // Start the DMA transfer on channel 0
}