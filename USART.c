#include "USART.h"
#include <stdint.h>

static void enUSARTClock(uint32_t USART_Num)
{
	//Enable RCC depending on USARTx
	if(USART_Num == 1)
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	if(USART_Num == 2)
		RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;	
}

static uint32_t getUSARTClockSpeed(uint32_t USART_Num)
{
	uint32_t AHB_Prescaler;
	
	//Get AHB Prescaler bits and format them
	uint32_t HPRE = (RCC->CFGR & RCC_CFGR_HPRE_Msk);
	HPRE = (HPRE>>4);
	
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
	
	if(USART_Num == 1)
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
	
	if(USART_Num == 2)
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

static void setBaudRate(uint32_t baudRate, uint32_t USART_Num)				//USART must be disabled
{	
	//Read OVER8 bit to determine baud rate equation
	uint32_t OVER8Bit = 0;
	if(USART_Num == 1)
	{
		OVER8Bit = ((USART1->CR1 & (1<<15))>>15);
		
		//Set USARTDIV based on equation
		if(OVER8Bit == 0)
			USART1->BRR = (getUSARTClockSpeed(USART_Num)/baudRate);	
		
		if(OVER8Bit == 1)
			USART1->BRR = ((2*getUSARTClockSpeed(USART_Num))/baudRate);
	}
	else if(USART_Num == 2)
	{
		OVER8Bit = ((USART2->CR1 & (1<<15))>>15);
		
		//Set USARTDIV based on equation
		if(OVER8Bit == 0)
			USART2->BRR = (getUSARTClockSpeed(USART_Num)/baudRate);	
		
		if(OVER8Bit == 1)
			USART2->BRR = ((2*getUSARTClockSpeed(USART_Num))/baudRate);
	}
}

static void enUSART(uint32_t USART_Num)
{
	if(USART_Num == 1)
	{
		//Enable UART
		USART1->CR1 |= USART_CR1_UE;
		
		//ENABLE TX & RX
		USART1->CR1 |= USART_CR1_RE | USART_CR1_TE;
	}
	
	if(USART_Num == 2)
	{
		//Enable UART
		USART2->CR1 |= USART_CR1_UE;
		
		//ENABLE TX & RX
		USART2->CR1 |= USART_CR1_RE | USART_CR1_TE;
	}
}

void USART_enInterupts(uint32_t USART_Num)
{
	if(USART_Num == 1)
	{
		//Enable Interupts USART side
		USART1->CR1 |= USART_CR1_TXEIE | USART_CR1_RXNEIE;
	}
	if(USART_Num == 2)
	{
		//Enable Interupts USART side
		USART2->CR1 |= USART_CR1_TXEIE | USART_CR1_RXNEIE;
	}
}

void usart_init(USART_T* USARTConfig)
{
	enUSARTClock(USARTConfig->USART_Num);
	setBaudRate(USARTConfig->baudRate, USARTConfig->USART_Num);
	enUSART(USARTConfig->USART_Num);
}
