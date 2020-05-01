#ifndef CUSTOM_USART_H
#define CUSTOM_USART_H

#include "stm32l432xx.h"

typedef struct {
	uint32_t USART_Num;
	uint32_t baudRate;
} USART_T;

static void enUSARTClock(uint32_t USART_Num);
static uint32_t getUSARTClockSpeed(uint32_t USART_Num);
static void setBaudRate(uint32_t baudRate, uint32_t USART_Num);		//USART must be disabled
static void enUSART(uint32_t USART_Num);

void USART_enInterupts(uint32_t USART_Num);					//requires user to implement irq handler
void usart_init(USART_T* USARTConfig);

#endif	//CUSTOM_USART_H

/*//IRQHandler Structure
void USARTx_IRQHandler(void)
{
	
}
*/
