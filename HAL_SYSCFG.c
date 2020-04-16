#include "HAL_SYSCFG.h"

static uint32_t SYSCFG_ClockEnabled(void)
{
	if((1 & RCC->APB2ENR) == 1)
	{
		return 1;
	}
	return 0;
}

static void enSYSCFG_Clock(void)
{
	if(SYSCFG_ClockEnabled() == 0)
	{
		RCC->APB2ENR |= (1<<0);
	}
}

void enEXTI_GPIO(GPIO_TypeDef *GPIO_Port, uint32_t GPIO_Pin)
{
	//Enable SYSCFG in RCC
	enSYSCFG_Clock();
	
	//Find Register from GPIO_Pin
	//Notes:
	//				SYSCFG_EXTICR[registerIndex]
	//
	//				SYSCFG_EXTICR[0] = SYSCFG_EXTICR1 in data-sheet
	uint32_t registerIndex;
	if(GPIO_Pin <= 3)
		registerIndex = 0;
	
	if(GPIO_Pin >= 4 && GPIO_Pin <= 7)
		registerIndex = 1;
	
	if(GPIO_Pin >= 8 && GPIO_Pin <= 11)
		registerIndex = 2;

	if(GPIO_Pin >= 12 && GPIO_Pin <= 15)
		registerIndex = 3;
	
	//Find Sector Configuration from GPIO_Port
	uint32_t sectConfig;
	if(GPIO_Port == GPIOA)
		sectConfig = 0;
	
	if(GPIO_Port == GPIOB)
		sectConfig = 1;
	
	if(GPIO_Port == GPIOC)
		sectConfig = 2;
	
	//GPIO_Pin determines the sector index
	
	//Config SYSCFG EXTI register
	SYSCFG->EXTICR[registerIndex] |= setMask(sectConfig, GPIO_Pin, SYSCFG_EXTI_SECTLEN);
	SYSCFG->EXTICR[registerIndex] &= ~(clearMask(sectConfig, GPIO_Pin, SYSCFG_EXTI_SECTLEN));
}
