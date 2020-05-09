#include "EXTI.h"

static void enEXTIForSYSCFG(GPIO_Type* GPIOConfig)
{
	//Enable SYSCFG clock
	RCC->APB2ENR |= (1<<0);
	
	//SYSCFG_EXTICR[EXTICR_Index] sectioned by pin number
	uint32_t EXTICR_Index = (GPIOConfig->pin/4);
	
	//Find Sector Configuration from GPIO_Port
	uint32_t sectConfig;
	if(GPIOConfig->port == GPIOA)
		sectConfig = 0;
	
	if(GPIOConfig->port == GPIOB)
		sectConfig = 1;
	
	if(GPIOConfig->port == GPIOC)
		sectConfig = 2;
	
	//Config SYSCFG EXTI register
	SYSCFG->EXTICR[EXTICR_Index] |= setMask(sectConfig, GPIOConfig->pin, SYSCFG_EXTI_SECTLEN);
	SYSCFG->EXTICR[EXTICR_Index] &= ~(clearMask(sectConfig, GPIOConfig->pin, SYSCFG_EXTI_SECTLEN));
}

static void setInteruptMask(EXTI_Type* EXTIConfig)
{
	//Interupt Mask Register 1
	if(EXTIConfig->line <= 31)
		EXTI->IMR1 |= (1<<EXTIConfig->line);
	
	//Interupt Mask Register 2
	if(EXTIConfig->line >= 32)
		EXTI->IMR2 |= (1<<(EXTIConfig->line - 32));
}

static void enRisingEdge(uint32_t EXTI_Line)
{
	//Rising Trigger Selection Register 1
	if(EXTI_Line <= 31)
		EXTI->RTSR1 |= (1<<EXTI_Line);
	
	//Rising Trigger Selection Register 2
	if(EXTI_Line >= 32)
		EXTI->RTSR2 |= (1<<(EXTI_Line - 32));
}

static void enFallingEdge(uint32_t EXTI_Line)
{
	//Falling Trigger Selection Register 1
	if(EXTI_Line <= 31)
		EXTI->FTSR1 |= (1<<EXTI_Line);
	
	//Falling Trigger Selection Register 2
	if(EXTI_Line >= 32)
		EXTI->FTSR2 |= (1<<(EXTI_Line - 32));
}


void extiInit(GPIO_Type* GPIOConfig, EXTI_Type* EXTIConfig, IRQn_Type irqNumber)
{
	//Enable EXTI through SYSCFG
	enEXTIForSYSCFG(GPIOConfig);
	
	setInteruptMask(EXTIConfig);
	extiSetEdge(EXTIConfig);
	
	NVIC_EnableIRQ(irqNumber);
}

void extiClearPending(uint32_t EXTI_Line)
{
	//Pending Register 1
	if(EXTI_Line <= 31)
		EXTI->PR1 |= (1<<EXTI_Line);
	
	//Pending Register 2
	if(EXTI_Line >= 32)
		EXTI->PR2 |= (1<<(EXTI_Line - 32));
}

void extiSetEdge(EXTI_Type* EXTIConfig)
{
	if(EXTIConfig->edgeTrigger & EXTI_EDGE_RISING)
		enRisingEdge(EXTIConfig->line);
	
	if(EXTIConfig->edgeTrigger & EXTI_EDGE_FALLING)
		enFallingEdge(EXTIConfig->line);
}
