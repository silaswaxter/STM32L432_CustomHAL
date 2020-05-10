#include "stm32l4xx.h"

#include <string.h>

#include "ClockControl.h"
#include "GPIO.h"
#include "EXTI.h"
#include "USART.h"
#include "DMA.h"
#include "I2C.h"

#define EM7180_Address 0x28
#define EM7180_ROMVersion 0x70 	//2 bytes long

//Clock Structs
SystemClock_T SYSCLK;
PLL_T PLL_SYSCLK;

//GPIO-EXTI Structs
GPIO_Type led3;
EXTI_Type extiB1;

//I2C-DMA Structs
DMA_T dma_I2C1_TX;
DMA_T dma_I2C1_RX;
char i2c_TestData_RX[16];

//Application Functions
void UART2_DMA_TestSetup(void);
void LEDInteruptInit(void);
void SystemClockInit(void);

void EM7180_I2C_ReadBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t byteReadNum);

int main()
{	
	SystemClockInit();		//Set SYSCLK @ 80MHz
	
	
	
	//Enable I2C
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;
	
	//Config I2C GPIO
	GPIO_Type gpio_I2C1_SDA;
	gpio_I2C1_SDA.port = GPIOB;
	gpio_I2C1_SDA.pin = 7;
	gpio_I2C1_SDA.mode = GPIO_MODE_ALTFUNC;
	gpio_I2C1_SDA.altFunctionNum = 4;
	gpio_I2C1_SDA.outputType = GPIO_OUTPUT_TYPE_DRAIN;
	gpio_I2C1_SDA.outputSpeed = GPIO_OUTPUT_SPEED_VERYHIGH;
	gpio_I2C1_SDA.pull = GPIO_PULL_UP;
	
	gpioInit(&gpio_I2C1_SDA);
	
	GPIO_Type gpio_I2C1_SCL;
	gpio_I2C1_SCL.port = GPIOB;
	gpio_I2C1_SCL.pin = 6;
	gpio_I2C1_SCL.mode = GPIO_MODE_ALTFUNC;
	gpio_I2C1_SCL.altFunctionNum = 4;
	gpio_I2C1_SCL.outputType = GPIO_OUTPUT_TYPE_DRAIN;
	gpio_I2C1_SCL.outputSpeed = GPIO_OUTPUT_SPEED_VERYHIGH;
	gpio_I2C1_SCL.pull = GPIO_PULL_UP;
	
	gpioInit(&gpio_I2C1_SCL);
	
	//Set I2C Speed
	I2C1->TIMINGR = I2C_TIMINGR_100khz;
	
	//Enable DMA Requests
	I2C1->CR1 |= I2C_CR1_RXDMAEN | I2C_CR1_TXDMAEN;
	
	//Enable AUTOEND (sends stop-condition after NBYTES)
	I2C1->CR2 |= I2C_CR2_AUTOEND;
	
	//Enable I2C Peripheral
	I2C1->CR1 |= I2C_CR1_PE;
	
	//Setup DMA
	dma_I2C1_RX.dmaNum = 2;
	dma_I2C1_RX.dmaChannelNum = 6;
	dma_I2C1_RX.dmaPeriph = DMA2_Channel6;
	dma_I2C1_RX.periphAddress = (uint32_t)&I2C1->RXDR;
	dma_I2C1_RX.memAddress = (uint32_t)&i2c_TestData_RX;
	dma_I2C1_RX.periphDataSize_Bits = (0x00);
	dma_I2C1_RX.memDataSize_Bits = (0x00);
	dma_I2C1_RX.numTransfersPerRequest = 2;
	dma_I2C1_RX.selChannelPeriph_Bits = (0x05);
	dma_I2C1_RX.priority = DMAPriority_Medium;
	dma_I2C1_RX.circMode = 1;
	dma_I2C1_RX.memIncr = 1;
	dma_I2C1_RX.periphIncr = 0;
	dma_I2C1_RX.readFromMem = 0;
	
	dmaConfig(&dma_I2C1_RX);
	dmaEnable(&dma_I2C1_RX);
	
	EM7180_I2C_ReadBytes(EM7180_Address, EM7180_ROMVersion, 2);
	
	while(1)
	{ 
		
	}	
}

void EM7180_I2C_ReadBytes(uint8_t SlaveAddress, uint8_t subAddress, uint8_t byteReadNum)
{
	//Set Slave Address
	I2C1->CR2 &= ~(0b1111111<<1);				//clear SlaveAddress bits
	I2C1->CR2 |= (SlaveAddress<<1);
	
	//set Number of Bytes
	I2C1->CR2 &= ~(I2C_CR2_NBYTES_Msk);		//clear NBYTES bits
	I2C1->CR2 |= (1<<16);
	
	//set Transfer Direction
	I2C1->CR2 &= ~(I2C_CR2_RD_WRN);				//Write
	
	//send start condition
	I2C1->CR2 |= I2C_CR2_START;
	
	//write SubAddress
	I2C1->TXDR = subAddress;
	
	//Since AUTOEND is enabled, STOP condition will be sent after NBYTES (1 in this case)
	
	//wait for Stop Condition
	while(!(I2C1->ISR & I2C_ISR_STOPF));
	
	//set Number of Bytes
	I2C1->CR2 &= ~(I2C_CR2_NBYTES_Msk);		//clear NBYTES bits
	I2C1->CR2 |= (byteReadNum<<16);
	
	//set Transfer Direction
	I2C1->CR2 |= (I2C_CR2_RD_WRN);				//Read		
	
	//send start condition
	I2C1->CR2 |= I2C_CR2_START;
	
	//wait for DMA_RX_TC Flag
	while(DMA2->ISR & (1<<21));
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

void UART2_DMA_TestSetup(void)
{
	GPIO_Type gpio_UART2_TX;
	gpio_UART2_TX.port = GPIOA;
	gpio_UART2_TX.pin = 2;
	gpio_UART2_TX.mode = GPIO_MODE_ALTFUNC;
	gpio_UART2_TX.altFunctionNum = 7;													//AF7
	gpio_UART2_TX.outputType = GPIO_OUTPUT_TYPE_PUSH;
	gpio_UART2_TX.outputSpeed = GPIO_OUTPUT_SPEED_VERYHIGH;
	
	gpioInit(&gpio_UART2_TX);
	
	GPIO_Type gpio_UART2_RX;
	gpio_UART2_RX.port = GPIOA;
	gpio_UART2_RX.pin = 3;
	gpio_UART2_RX.mode = GPIO_MODE_ALTFUNC;
	gpio_UART2_RX.altFunctionNum = 7;													//AF7
	gpio_UART2_RX.outputType = GPIO_OUTPUT_TYPE_PUSH;
	gpio_UART2_RX.outputSpeed = GPIO_OUTPUT_SPEED_VERYHIGH;
	
	gpioInit(&gpio_UART2_RX);
	
	USART_T uart2;
	uart2.usartPeriph = USART2;
	uart2.baudRate = 9600;
	uart2.usartNum = 2;
	uart2.dmaEnabled_Rx = 1;
	uart2.dmaEnabled_Tx = 1;
	
	usartInit(&uart2);	
	
	//usartEnInterupts(uart2.usartPeriph);
	//NVIC_EnableIRQ(USART2_IRQn);
	
	//TX Test Data
	char txData[] = "1234567890";
	char rxData;
	
	//Enable DMA
	DMA_T dma_UART2_TX;
	dma_UART2_TX.dmaNum = 1;
	dma_UART2_TX.dmaChannelNum = 7;
	dma_UART2_TX.dmaPeriph = DMA1_Channel7;
	dma_UART2_TX.periphAddress = (uint32_t)&USART2->TDR;
	dma_UART2_TX.memAddress = (uint32_t)txData;
	dma_UART2_TX.numTransfersPerRequest = (strlen(txData));
	dma_UART2_TX.selChannelPeriph_Bits = (0x02);
	dma_UART2_TX.circMode = 0;
	dma_UART2_TX.memIncr = 1;
	dma_UART2_TX.periphIncr = 0;
	dma_UART2_TX.readFromMem = 1;
	
	dmaConfig(&dma_UART2_TX);
	
	DMA_T dma_UART2_RX;
	dma_UART2_RX.dmaNum = 1;
	dma_UART2_RX.dmaChannelNum = 6;
	dma_UART2_RX.dmaPeriph = DMA1_Channel6;
	dma_UART2_RX.periphAddress = (uint32_t)&USART2->RDR;
	dma_UART2_RX.memAddress = (uint32_t)&rxData;
	dma_UART2_RX.numTransfersPerRequest = 1;
	dma_UART2_RX.selChannelPeriph_Bits = (0x02);
	dma_UART2_RX.circMode = 1;
	dma_UART2_RX.memIncr = 0;
	dma_UART2_RX.periphIncr = 0;
	dma_UART2_RX.readFromMem = 0;
	
	dmaConfig(&dma_UART2_RX);
	
	dmaEnable(&dma_UART2_TX);
	dmaEnable(&dma_UART2_RX);
}


/*
void EXTI1_IRQHandler()
{
	//Clear Pending Interupt
	extiClearPending(extiB1.line);
	
	//Turn On LED
	gpioWrite(&led3, HIGH);
}
*/

void LEDInteruptInit(void)
{
	//Led GPIO-Pin Setup
	led3.port = GPIOB;
	led3.pin = 3;
	led3.mode = GPIO_MODE_OUTPUT;
	led3.outputType = GPIO_OUTPUT_TYPE_PUSH;
	led3.outputSpeed = GPIO_OUTPUT_SPEED_MEDIUM;
	led3.pull = GPIO_PULL_NONE;
	
	gpioInit(&led3);

	gpioWrite(&led3, LOW);
	
	//Interupt GPIO-Pin Setup
	GPIO_Type interuptGPIO;
	interuptGPIO.port = GPIOB;
	interuptGPIO.pin = 1;
	interuptGPIO.mode = GPIO_MODE_OUTPUT;
	interuptGPIO.outputType = GPIO_OUTPUT_TYPE_PUSH;
	interuptGPIO.outputSpeed = GPIO_OUTPUT_SPEED_MEDIUM;
	interuptGPIO.pull = GPIO_PULL_NONE;
	
	gpioInit(&interuptGPIO);

	//EXTI External Interupt Setup
	extiB1.line = 1;
	extiB1.edgeTrigger = EXTI_EDGE_RISING;
	extiB1.mode = EXTI_MODE_INTERUPT;
	
	extiInit(&interuptGPIO, &extiB1, EXTI1_IRQn);
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
