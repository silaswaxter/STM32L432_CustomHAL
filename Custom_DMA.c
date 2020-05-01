#include "Custom_DMA.h"

static inline void enDMAClock(uint32_t DMA_Num)
{
	(DMA_Num == 1) ? (RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN) : (RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN);
}

static void selChannelPeriph(DMA_Channel_T* DMAConfig)
{
	if(DMAConfig->DMA_Num == 1)
	{
		DMA1_CSELR->CSELR |= (DMAConfig->selChannelPeriph_Bits<<(4*(DMAConfig->DMA_ChannelNum - 1)));
	}
	if(DMAConfig->DMA_Num == 2)
	{
		DMA2_CSELR->CSELR |= (DMAConfig->selChannelPeriph_Bits<<(4*(DMAConfig->DMA_ChannelNum - 1)));
	}
}


void dma_init(DMA_Channel_T* DMAConfig)
{
	enDMAClock(DMAConfig->DMA_Num);
	
	//Set DMA Peripheral Address
	DMAConfig->DMAx_Channeln_Access->CPAR = DMAConfig->PeriphAddress;
	
	//Set DMA Memory Address
	DMAConfig->DMAx_Channeln_Access->CMAR = DMAConfig->MemAddress;
	
	//Set DMA Number of Data to Transfer
	DMAConfig->DMAx_Channeln_Access->CNDTR = DMAConfig->NumDataToTransfer;
	
	//Set DMA Channel Event Source
	selChannelPeriph(DMAConfig);
	
	//Set DMA Boolean Settings
	if(DMAConfig->CircularMode == 1)
		DMAConfig->DMAx_Channeln_Access->CCR |= DMA_CCR_CIRC;
	
	if(DMAConfig->MemIncrement == 1)
		DMAConfig->DMAx_Channeln_Access->CCR |= DMA_CCR_MINC;
	
	if(DMAConfig->PeriphIncrement == 1)
		DMAConfig->DMAx_Channeln_Access->CCR |= DMA_CCR_PINC;

	if(DMAConfig->ReadFromMemory == 1)
		DMAConfig->DMAx_Channeln_Access->CCR |= DMA_CCR_DIR;
}

void startDMA(DMA_Channel_T* DMAConfig)
{
	DMAConfig->DMAx_Channeln_Access->CCR |= DMA_CCR_EN;
}


void stopDMA(DMA_Channel_T* DMAConfig)
{
	DMAConfig->DMAx_Channeln_Access->CCR &= ~DMA_CCR_EN;
}
