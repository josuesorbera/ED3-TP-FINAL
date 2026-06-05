#ifndef DMA_H
#define DMA_H

// Libraries inclusion
#include <stdio.h>
#include "LPC17xx.h"
#include "lpc17xx_gpdma.h"

// Macro definitions


// Function prototypes
void DMA_Config(GPDMA_LLI_T* lli);

#endif // DMA_H
