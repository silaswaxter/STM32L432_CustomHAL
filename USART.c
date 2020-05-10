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

static void setBaudRate(USART_T* USARTConfig)				//USART must be disabled
{	
	//Read OVER8 bit to determine baud rate equation
	uint32_t OVER8Bit = 0;
	
	OVER8Bit = ((USARTConfig->usartPeriph->CR1 & (1<<15))>>15);
	
	//Get USART Peripheral's Clock Speed in MHz
	uint32_t usartPeriphClockSpeed = 0;
	
	if(USARTConfig->usartNum == 1)
		usartPeriphClockSpeed = getAPBCLK(1);
	
	if(USARTConfig->usartNum == 2)
		usartPeriphClockSpeed = getAPBCLK(2);
	
	//Set USARTDIV based on equation
	if(OVER8Bit == 0)
		USARTConfig->usartPeriph->BRR = (usartPeriphClockSpeed/USARTConfig->baudRate);	
	
	if(OVER8Bit == 1)
		USARTConfig->usartPeriph->BRR = ((2*usartPeriphClockSpeed)/USARTConfig->baudRate);
	
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
