#ifndef USART_H
#define USART_H

#include "ClockControl.h"
#include "stm32l432xx.h"

typedef struct {
	USART_TypeDef* usartPeriph;				//usart access (USART1, USART2, etc.)
	uint32_t usartNum;								//usart number (x in USARTx)
	uint32_t baudRate;
	uint32_t dmaEnabled_Tx;
	uint32_t dmaEnabled_Rx;
} USART_T;

static void enUSARTClock(uint32_t usartNum);
static void setBaudRate(USART_T* USARTConfig);		//USART must be disabled
static void enUSART(USART_TypeDef* usartPeriph);

void usartEnDMA_TX(USART_T* USARTConfig);
void usartEnDMA_RX(USART_T* USARTConfig);
void usartEnInterupts(USART_TypeDef* usartPeriph);					//requires user to implement irq handler
void usartInit(USART_T* USARTConfig);

#endif	//USART_H

/*
//IRQHandler Structure
void USARTx_IRQHandler(void)
{
	
}
*/
