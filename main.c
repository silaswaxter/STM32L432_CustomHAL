#include "stm32l4xx.h"

#include <string.h>

#include "HAL_GPIO.h"
#include "HAL_SYSCFG.h"
#include "HAL_EXTI.h"
#include "ClockControl.h"
#include "Custom_USART.h"
#include "Custom_DMA.h"

//Clock Structs
SystemClock_T SYSCLK;
PLL_T PLL_SYSCLK;

//GPIO_Type led3;
//EXTI_Type extiB1;
//void LEDInteruptInit(void);
void UART2Init(void);
void SystemClockInit(void);

int main()
{	
	SystemClockInit();		//Set SYSCLK @ 80MHz
	
	UART2Init();
	
	//TX Test Data
	char txData[] = "1234567890";
	char rxData;
	
	//Enable DMA
		//DMA1_Channel6 = USART2_RX
		//DMA1_Channel7 = USART2_TX
	DMA_Channel_T dma_UART2_TX;
	dma_UART2_TX.DMA_Num = 1;
	dma_UART2_TX.DMA_ChannelNum = 7;
	dma_UART2_TX.DMAx_Channeln_Access = DMA1_Channel7;
	dma_UART2_TX.PeriphAddress = (uint32_t)&USART2->TDR;
	dma_UART2_TX.MemAddress = (uint32_t)txData;
	dma_UART2_TX.NumDataToTransfer = (strlen(txData));
	dma_UART2_TX.selChannelPeriph_Bits = (0x02);
	dma_UART2_TX.CircularMode = 0;
	dma_UART2_TX.MemIncrement = 1;
	dma_UART2_TX.PeriphIncrement = 0;
	dma_UART2_TX.ReadFromMemory = 1;
	
	dma_init(&dma_UART2_TX);
	
	DMA_Channel_T dma_UART2_RX;
	dma_UART2_RX.DMA_Num = 1;
	dma_UART2_RX.DMA_ChannelNum = 6;
	dma_UART2_RX.DMAx_Channeln_Access = DMA1_Channel6;
	dma_UART2_RX.PeriphAddress = (uint32_t)&USART2->RDR;
	dma_UART2_RX.MemAddress = (uint32_t)&rxData;
	dma_UART2_RX.NumDataToTransfer = 1;
	dma_UART2_RX.selChannelPeriph_Bits = (0x02);
	dma_UART2_RX.CircularMode = 1;
	dma_UART2_RX.MemIncrement = 0;
	dma_UART2_RX.PeriphIncrement = 0;
	dma_UART2_RX.ReadFromMemory = 0;
	
	dma_init(&dma_UART2_RX);
	
	//Enable USART2 DMA Tx/RX
	USART2->CR3 |= USART_CR3_DMAT;	
	USART2->CR3 |= USART_CR3_DMAR;
	
	startDMA(&dma_UART2_TX);
	startDMA(&dma_UART2_RX);
	
	while(1)
	{ 
		
	}	
}

/*
void USART2_IRQHandler()
{
	//Check IRQtrigger: RXE
	if(USART2->ISR & USART_ISR_RXNE)
	{
		char testSingleBuffer = USART2->RDR;		//read received data
		USART2->TDR = testSingleBuffer;					//send received data		
		while(!(USART2->ISR & USART_ISR_TC));		//wait for transfer to complete
	}
	
	//Check IRQtrigger: TXE
	if(USART2->ISR & USART_ISR_RXNE)
	{
		
	}
	
	//Check IRQtrigger: ORE
	if(USART2->ISR & USART_ISR_ORE)
	{
		
	}
}
*/

void UART2Init(void)
{
	GPIO_Type gpio_UART1_TX;
	gpio_UART1_TX.port = GPIOA;
	gpio_UART1_TX.pin = 2;
	gpio_UART1_TX.mode = GPIO_MODE_ALTFUNC;
	gpio_UART1_TX.altFunctionNum = 7;													//AF7
	gpio_UART1_TX.output_Type = GPIO_OUTPUT_TYPE_PUSH;
	gpio_UART1_TX.output_Speed = GPIO_OUTPUT_SPEED_VERYHIGH;
	
	gpio_init(&gpio_UART1_TX);
	
	GPIO_Type gpio_UART1_RX;
	gpio_UART1_RX.port = GPIOA;
	gpio_UART1_RX.pin = 3;
	gpio_UART1_RX.mode = GPIO_MODE_ALTFUNC;
	gpio_UART1_RX.altFunctionNum = 7;													//AF7
	gpio_UART1_RX.output_Type = GPIO_OUTPUT_TYPE_PUSH;
	gpio_UART1_RX.output_Speed = GPIO_OUTPUT_SPEED_VERYHIGH;
	
	gpio_init(&gpio_UART1_RX);
	
	USART_T uart2;
	uart2.baudRate = 9600;
	uart2.USART_Num = 2;
	
	usart_init(&uart2);	
	
	USART_enInterupts(uart2.USART_Num);
	//NVIC_EnableIRQ(USART2_IRQn);
}

void SystemClockInit(void)
{
	PLL_T PLL_SYSCLK;
	PLL_SYSCLK.PLLClockSource = ClockSource_HSE;
	PLL_SYSCLK.PLLM = PLLM_1;
	PLL_SYSCLK.PLLN = 20;
	PLL_SYSCLK.PLLR = PLLR_2;
	
	SystemClock_T SYSCLK;
	SYSCLK.AHBPrescaler = AHB_1;
	SYSCLK.APB1Prescaler = APB_1;
	SYSCLK.APB2Prescaler = APB_1;
	SYSCLK.PLL_Configuration = &PLL_SYSCLK;
	SYSCLK.SystemClockSource = ClockSource_PLL;
	SYSCLK.TargetSystemClockSpeedMHZ = 8;
	
	setSystemClock(&SYSCLK);
}

/*
void EXTI1_IRQHandler()
{
	//Clear Pending Interupt
	exti_clearPending(extiB1.line);
	
	//Turn On LED
	gpio_writePin(&led3, HIGH);
}

void LEDInteruptInit(void)
{
	//Led GPIO-Pin Setup
	led3.port = GPIOB;
	led3.pin = 3;
	led3.mode = GPIO_MODE_OUTPUT;
	led3.output_Type = GPIO_OUTPUT_TYPE_PUSH;
	led3.output_Speed = GPIO_OUTPUT_SPEED_MEDIUM;
	led3.pull = GPIO_PULL_NONE;
	
	gpio_init(&led3);
	
	//Interupt GPIO-Pin Setup
	GPIO_Type interuptGPIO;
	interuptGPIO.port = GPIOB;
	interuptGPIO.pin = 1;
	interuptGPIO.mode = GPIO_MODE_OUTPUT;
	interuptGPIO.output_Type = GPIO_OUTPUT_TYPE_PUSH;
	interuptGPIO.output_Speed = GPIO_OUTPUT_SPEED_MEDIUM;
	interuptGPIO.pull = GPIO_PULL_NONE;
	
	gpio_init(&interuptGPIO);

	//EXTI External Interupt Setup
	extiB1.line = 1;
	extiB1.edgeTrigger = EXTI_EDGE_RISING;
	extiB1.mode = EXTI_MODE_INTERUPT;

	//Interupt Initialization
	enEXTI_GPIO(interuptGPIO.port, interuptGPIO.pin);
	
	//EXTI Initialization
	exti_init(&extiB1, EXTI1_IRQn);
}
*/
