#include <stdio.h>
#include "DMA.h"

void DMA_Config(void) {
    GPDMA_Init();

    //Configuramos la estructura LLI circular apuntando a sí misma
    lli.srcAddr = (uint32_t)&(LPC_ADC->ADGDR);
    lli.dstAddr = (uint32_t)adc_buffer;
    lli.nextLLI = (uint32_t)&lli;
    // Control LLI: 64 muestras | Origen 32 bits | Destino 32 bits | Auto-incremento en RAM | Int Enable
    lli.control = (SAMPLES | (2 << 18) | (2 << 21) | (1 << 27) | (1UL << 31));

    //Configuramos el canal
    GPDMA_Endpoint_T srcCfg = {0};
    srcCfg.width = GPDMA_WORD;
    srcCfg.burst = GPDMA_BSIZE_1;
    srcCfg.increment = DISABLE;

    GPDMA_Endpoint_T dstCfg = {0};
    dstCfg.width = GPDMA_WORD;
    dstCfg.burst = GPDMA_BSIZE_1;
    dstCfg.increment = ENABLE;

    GPDMA_Channel_CFG_T configDMA = {0};
    configDMA.channelNum = GPDMA_CH_0;
    configDMA.transferSize = SAMPLES;
    configDMA.type = GPDMA_P2M;
    configDMA.srcMemAddr = (uint32_t)&(LPC_ADC->ADGDR);
    configDMA.dstMemAddr = (uint32_t)adc_buffer;
    configDMA.srcConn = GPDMA_ADC;
    configDMA.src = srcCfg;
    configDMA.dst = dstCfg;
    configDMA.intTC = ENABLE;
    configDMA.intErr = ENABLE;

    //Le pasamos el puntero de la LLI al driver
    configDMA.linkedList = (uint32_t)&lli;

    GPDMA_SetupChannel(&configDMA);

    NVIC_EnableIRQ(DMA_IRQn);
    NVIC_SetPriority(DMA_IRQn, 1);
}


void DMA_IRQHandler(void) { //Handler para actualizar cada vez que interrumpe DMA
    if (GPDMA_IntGetStatus(GPDMA_INTTC, GPDMA_CH_0)) {
        GPDMA_ClearIntPending(GPDMA_CLR_INTTC, GPDMA_CH_0);

        uint32_t suma_total = 0;

        for(int i = 0; i < SAMPLES; i++) {
            suma_total += ((adc_buffer[i] >> 4) & 0xFFF);
        }

        //Promedio de las 64 muestras
        adcValue = (uint16_t)(suma_total / SAMPLES);

        //Se mueve el servo
        servo_Position(adcValue);
    }

    if (GPDMA_IntGetStatus(GPDMA_INTERR, GPDMA_CH_0)) {
        GPDMA_ClearIntPending(GPDMA_CLR_INTERR, GPDMA_CH_0);
    }
}