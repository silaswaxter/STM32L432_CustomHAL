#include "DMA.h"

static inline void enDMAClock(uint32_t DMA_Num)
{
	(DMA_Num == 1) ? (RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN) : (RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN);
}

static void selChannelPeriph(DMA_T* DMAConfig)
{
	if(DMAConfig->dmaNum == 1)
	{
		DMA1_CSELR->CSELR |= (DMAConfig->selChannelPeriph_Bits<<(4*(DMAConfig->dmaChannelNum - 1)));
	}
	if(DMAConfig->dmaNum == 2)
	{
		DMA2_CSELR->CSELR |= (DMAConfig->selChannelPeriph_Bits<<(4*(DMAConfig->dmaChannelNum - 1)));
	}
}


void dmaConfig(DMA_T* DMAConfig)
{
	enDMAClock(DMAConfig->dmaNum);
	
	//Set DMA Peripheral Address
	DMAConfig->dmaPeriph->CPAR = DMAConfig->PeriphAddress;
	
	//Set DMA Memory Address
	DMAConfig->dmaPeriph->CMAR = DMAConfig->MemAddress;
	
	//Set DMA Number of Data to Transfer
	DMAConfig->dmaPeriph->CNDTR = DMAConfig->NumDataToTransfer;
	
	//Set DMA Channel Event Source
	selChannelPeriph(DMAConfig);
	
	//Set DMA Boolean Settings
	if(DMAConfig->CircularMode)
		DMAConfig->dmaPeriph->CCR |= DMA_CCR_CIRC;
	
	if(DMAConfig->MemIncrement)
		DMAConfig->dmaPeriph->CCR |= DMA_CCR_MINC;
	
	if(DMAConfig->PeriphIncrement)
		DMAConfig->dmaPeriph->CCR |= DMA_CCR_PINC;

	if(DMAConfig->ReadFromMemory)
		DMAConfig->dmaPeriph->CCR |= DMA_CCR_DIR;
}

void dmaEnable(DMA_T* DMAConfig)
{
	DMAConfig->dmaPeriph->CCR |= DMA_CCR_EN;
}

void dmaDisable(DMA_T* DMAConfig)
{
	DMAConfig->dmaPeriph->CCR &= ~DMA_CCR_EN;
}
