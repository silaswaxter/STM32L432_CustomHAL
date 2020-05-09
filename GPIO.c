#include "GPIO.h"
#include <stdint.h>

static void enGPIOPortClock(GPIO_Type *GPIOConfig)
{	
	if (GPIOConfig->port == GPIOA)
		RCC->AHB2ENR |= (1<<0);
	
	if (GPIOConfig->port == GPIOB)
		RCC->AHB2ENR |= (1<<1);
	
	if (GPIOConfig->port == GPIOC)
		RCC->AHB2ENR |= (1<<2);
}

static void setMode(GPIO_Type *GPIOConfig)
{
	GPIOConfig->port->MODER |= setMask(GPIOConfig->mode, GPIOConfig->pin, GPIO_MODE_SECTLEN);	
	GPIOConfig->port->MODER &= ~(clearMask(GPIOConfig->mode, GPIOConfig->pin, GPIO_MODE_SECTLEN));
}

static void setOutputType(GPIO_Type *GPIOConfig)
{
	GPIOConfig->port->OTYPER |= setMask(GPIOConfig->outputType, GPIOConfig->pin, GPIO_OUTPUT_TYPE_SECTLEN);
	GPIOConfig->port->OTYPER &= ~(clearMask(GPIOConfig->outputType, GPIOConfig->pin, GPIO_OUTPUT_TYPE_SECTLEN));
}

static void setOutputSpeed(GPIO_Type *GPIOConfig)
{
	GPIOConfig->port->OSPEEDR |= setMask(GPIOConfig->outputSpeed, GPIOConfig->pin, GPIO_OUTPUT_SPEED_SECTLEN);
	GPIOConfig->port->OSPEEDR &= ~(clearMask(GPIOConfig->outputSpeed, GPIOConfig->pin, GPIO_OUTPUT_SPEED_SECTLEN));
}

static void setPull(GPIO_Type *GPIOConfig)
{
	GPIOConfig->port->PUPDR |= setMask(GPIOConfig->pull, GPIOConfig->pin, GPIO_PULL_SECTLEN);
	GPIOConfig->port->PUPDR &= ~(clearMask(GPIOConfig->pull, GPIOConfig->pin, GPIO_PULL_SECTLEN));
}

static void setAltFunction(GPIO_Type *GPIOConfig)
{
	//AFR:	HigherReg [1] or LowerReg [0] register 
	uint32_t AFR_Index = 0;
	
	//AFR:  HigherReg[1] bits are shifted from pin number by 8 (8=0, 9=1, etc.)
	uint32_t AFR_CorrectedPin = GPIOConfig->pin;
	
	//if HigherReg[1]
	if(AFR_CorrectedPin >= 8)
	{
		AFR_Index = 1;
		AFR_CorrectedPin -= 8;		//correct pin
	}
	
	GPIOConfig->port->AFR[AFR_Index] |= setMask(GPIOConfig->altFunctionNum, AFR_CorrectedPin, GPIO_ALTFUNCTION_SECTLEN);
	GPIOConfig->port->AFR[AFR_Index] &= ~(clearMask(GPIOConfig->altFunctionNum, AFR_CorrectedPin, GPIO_ALTFUNCTION_SECTLEN));
	
}
void gpioInit(GPIO_Type *GPIOConfig)
{
	//Enable Clock
	enGPIOPortClock(GPIOConfig);
	
	//set GPIO_MODE
	setMode(GPIOConfig);
	
	//set GPIO_OUTPUT_TYPE
	setOutputType(GPIOConfig);
	
	//set GPIO_OUTPUT_SPEED
	setOutputSpeed(GPIOConfig);
	
	//set pull
	setPull(GPIOConfig);
	
	//set Alternate_Function
	if (GPIOConfig->mode == GPIO_MODE_ALTFUNC)
		setAltFunction(GPIOConfig);
	
}

void gpioWrite(GPIO_Type *GPIOConfig, uint32_t state)
{
	//Set Register
	if(state == 1)
		GPIOConfig->port->BSRR |= (1<<(GPIOConfig->pin));
	
	//Set Register
	if(state == 0)
		GPIOConfig->port->BSRR |= (1<<((GPIOConfig->pin) + 16));
}
