#ifndef DMA_H
#define DMA_H

#include "stm32l432xx.h"

typedef enum {
	DMAPriority_Low = (0x00),
	DMAPriority_Medium = (0x01),
	DMAPriority_High = (0x02),
	DMAPriority_VeryHigh = (0x03)
} DMAPriority_T;

typedef struct {
	//DMA Selection
	uint32_t dmaNum;									//for DMA clock enable
	uint32_t dmaChannelNum;
	DMA_Channel_TypeDef* dmaPeriph;		//DMAx_Channeln
	//DMA Settings
	uint32_t MemAddress;
	uint32_t PeriphAddress;
	uint32_t MemDataSize_Bits;
	uint32_t PeriphDataSize_Bits;
	uint32_t NumDataToTransfer; 			//Number of Data-Transfers in one cycle
	uint32_t selChannelPeriph_Bits; 	//bits for mapping peripheral to channel
	DMAPriority_T Priority;	
	//Boolean Settings
	uint32_t CircularMode;
	uint32_t MemIncrement; 						//after x[0] is wrote, write to x[1]?
	uint32_t PeriphIncrement;
	uint32_t ReadFromMemory;					//When 1, dma: mem->periph (even when MEM2MEM or PER2PER is enabled [these are naming conventions for addresses])
} DMA_T;

static inline void enDMAClock(uint32_t DMA_Num);
static void selChannelPeriph(DMA_T* DMAConfig);		//selects peripheral for channel	(e.g. usart2_rx or usart1_rx)

void dmaConfig(DMA_T* DMAConfig);					//Config instead of Init because it doesn't enable periph 
void dmaEnable(DMA_T* DMAConfig);
void dmaDisable(DMA_T* DMAConfig);

#endif //DMA_H
