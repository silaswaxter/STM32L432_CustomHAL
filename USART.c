#include "USART.h"
#include <stdint.h>

static void enUSARTClock(uint32_t usartNum)
{
	//Enable RCC depending on USARTx
	if(usartNum == 1)
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	if(usartNum == 2)
		RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;	
}

static uint32_t getUSARTClockSpeed(uint32_t usartNum)
{
	uint32_t AHB_Prescaler;
	
	//Get AHB Prescaler bits and format them
	uint32_t HPRE = ((RCC->CFGR & RCC_CFGR_HPRE_Msk)>>4);
	
	//Convert bits to config
	if(!(HPRE & 0x08))
		AHB_Prescaler = 1;
	
	else if (HPRE == 0x08)
		AHB_Prescaler = 2;
	
	else if (HPRE == 0x09)
		AHB_Prescaler = 4;
	
	else if (HPRE == 0x0A)
		AHB_Prescaler = 8;
	
	else if (HPRE == 0x0B)
		AHB_Prescaler = 16;
	
	else if (HPRE == 0x0C)
		AHB_Prescaler = 64;
	
	else if (HPRE == 0x0D)
		AHB_Prescaler = 128;
	
	else if (HPRE == 0x0E)
		AHB_Prescaler = 256;
	
	else if (HPRE == 0x0F)
		AHB_Prescaler = 512;
	
	if(usartNum == 1)
	{
		uint32_t APB1_Prescaler;
		
		//Get APB1_Prescaler bits and format them
		uint32_t PPRE1 = (RCC->CFGR & RCC_CFGR_PPRE1_Msk);
		PPRE1 = (PPRE1>>8);						//Move bits to 0
	
		//Convert bits to config
		if(!(PPRE1 & 0x04))
			APB1_Prescaler = 1;
		
		else if (PPRE1 == 0x04)
			APB1_Prescaler = 2;
		
		else if (PPRE1 == 0x05)
			APB1_Prescaler = 4;
			
		else if (PPRE1 == 0x06)
			APB1_Prescaler = 8;
		
		else if (PPRE1 == 0x07)
			APB1_Prescaler = 16;
		
		return ((SystemCoreClock/AHB_Prescaler)/APB1_Prescaler);
	}
	
	if(usartNum == 2)
	{
		uint32_t APB2_Prescaler;
		
		//Get formated APB1_Prescaler
		uint32_t PPRE2 = (RCC->CFGR & RCC_CFGR_PPRE2_Msk);
		PPRE2 = (PPRE2>>11);					//Move bits to 0
		
		//Convert bits to config
		if(!(PPRE2 & 0x04))
			APB2_Prescaler = 1;
		
		else if (PPRE2 == 0x04)
			APB2_Prescaler = 2;
		
		else if (PPRE2 == 0x05)
			APB2_Prescaler = 4;
			
		else if (PPRE2 == 0x06)
			APB2_Prescaler = 8;
		
		else if (PPRE2 == 0x07)
			APB2_Prescaler = 16;
		
		return ((SystemCoreClock/AHB_Prescaler)/APB2_Prescaler);
	}
	return 0;			//ERROR
}

static void setBaudRate(USART_T* USARTConfig)				//USART must be disabled
{	
	//Read OVER8 bit to determine baud rate equation
	uint32_t OVER8Bit = 0;
	
	OVER8Bit = ((USARTConfig->usartPeriph->CR1 & (1<<15))>>15);
		
	//Set USARTDIV based on equation
	if(OVER8Bit == 0)
		USARTConfig->usartPeriph->BRR = (getUSARTClockSpeed(USARTConfig->usartNum)/USARTConfig->baudRate);	
	
	if(OVER8Bit == 1)
		USARTConfig->usartPeriph->BRR = ((2*getUSARTClockSpeed(USARTConfig->usartNum))/USARTConfig->baudRate);
}

static void enUSART(USART_TypeDef* usartPeriph)
{
	//Enable UART
	usartPeriph->CR1 |= USART_CR1_UE;
	
	//ENABLE TX & RX
	usartPeriph->CR1 |= USART_CR1_RE | USART_CR1_TE;
}

void usartEnDMA_TX(USART_T* USARTConfig)
{
	USARTConfig->usartPeriph->CR3 |= USART_CR3_DMAT;
}

void usartEnDMA_RX(USART_T* USARTConfig)
{
	USARTConfig->usartPeriph->CR3 |= USART_CR3_DMAR;
}

void usartEnInterupts(USART_TypeDef* usartPeriph)
{
	//Enable Interupts USART side
	usartPeriph->CR1 |= USART_CR1_TXEIE | USART_CR1_RXNEIE;
}

void usartInit(USART_T* USARTConfig)
{
	enUSARTClock(USARTConfig->usartNum);
	setBaudRate(USARTConfig);
	enUSART(USARTConfig->usartPeriph);
	
	if(USARTConfig->dmaEnabled_Tx)
		usartEnDMA_TX(USARTConfig);

	if(USARTConfig->dmaEnabled_Rx)
		usartEnDMA_RX(USARTConfig);
}
