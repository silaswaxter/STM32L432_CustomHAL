#include "HAL_GPIO.h"
#include <stdint.h>

static uint32_t portRCCEnabled(uint32_t gpioPortPos)
{
	if(((gpioPortPos & RCC->AHB2ENR)>>gpioPortPos) == 1)
	{
		return 1;
	}
	return 0;
}

static void enGPIOPortClock(GPIO_TypeDef *port)
{	
	if (port == GPIOA)
	{
		if(portRCCEnabled(RCC_AHB2ENR_GPIOAEN_Pos) == 0)
		{
			RCC->AHB2ENR |= (1<<0);
		}
	}
	
	if (port == GPIOB)
	{
		if(portRCCEnabled(RCC_AHB2ENR_GPIOBEN_Pos) == 0)
		{
			RCC->AHB2ENR |= (1<<1);
		}
	}
	
	if (port == GPIOC)
	{
		if(portRCCEnabled(RCC_AHB2ENR_GPIOCEN_Pos) == 0)
		{
			RCC->AHB2ENR |= (1<<2);
		}
	}
}

static void set_mode(GPIO_Type *IOPin)
{
	switch (IOPin->mode)
	{
		case GPIO_MODE_INPUT:
			IOPin->port->MODER |= setMask(GPIO_MODE_INPUT, IOPin->pin, GPIO_MODE_SECTLEN);
			IOPin->port->MODER &= ~(clearMask(GPIO_MODE_INPUT, IOPin->pin, GPIO_MODE_SECTLEN));
			break;
		
		case GPIO_MODE_OUTPUT:
			IOPin->port->MODER |= setMask(GPIO_MODE_OUTPUT, IOPin->pin, GPIO_MODE_SECTLEN);
			IOPin->port->MODER &= ~(clearMask(GPIO_MODE_OUTPUT, IOPin->pin, GPIO_MODE_SECTLEN));
			break;
		
		case GPIO_MODE_ALTFUNC:
			IOPin->port->MODER |= setMask(GPIO_MODE_ALTFUNC, IOPin->pin, GPIO_MODE_SECTLEN);
			IOPin->port->MODER &= ~(clearMask(GPIO_MODE_ALTFUNC, IOPin->pin, GPIO_MODE_SECTLEN));
			break;
		
		case GPIO_MODE_ANALOG:
			IOPin->port->MODER |= setMask(GPIO_MODE_ANALOG, IOPin->pin, GPIO_MODE_SECTLEN);
			IOPin->port->MODER &= ~(clearMask(GPIO_MODE_ANALOG, IOPin->pin, GPIO_MODE_SECTLEN));
			break;
	}
}

static void set_outputType(GPIO_Type *IOPin)
{
	switch (IOPin->output_Type)
	{
		case GPIO_OUTPUT_TYPE_PUSH:
			IOPin->port->MODER |= setMask(GPIO_OUTPUT_TYPE_PUSH, IOPin->pin, GPIO_OUTPUT_TYPE_SECTLEN);
			IOPin->port->MODER &= ~(clearMask(GPIO_OUTPUT_TYPE_PUSH, IOPin->pin, GPIO_OUTPUT_TYPE_SECTLEN));
			break;
		
		case GPIO_OUTPUT_TYPE_DRAIN:
			IOPin->port->MODER |= setMask(GPIO_OUTPUT_TYPE_DRAIN, IOPin->pin, GPIO_OUTPUT_TYPE_SECTLEN);
			IOPin->port->MODER &= ~(clearMask(GPIO_OUTPUT_TYPE_DRAIN, IOPin->pin, GPIO_OUTPUT_TYPE_SECTLEN));
			break;
	}
}

static void set_outputSpeed(GPIO_Type *IOPin)
{
	switch (IOPin->output_Speed)
	{
		case GPIO_OUTPUT_SPEED_LOW:
			IOPin->port->OSPEEDR |= setMask(GPIO_OUTPUT_SPEED_LOW, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN);
			IOPin->port->OSPEEDR &= ~(clearMask(GPIO_OUTPUT_SPEED_LOW, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN));
			break;
		
		case GPIO_OUTPUT_SPEED_MEDIUM:
			IOPin->port->OSPEEDR |= setMask(GPIO_OUTPUT_SPEED_MEDIUM, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN);
			IOPin->port->OSPEEDR &= ~(clearMask(GPIO_OUTPUT_SPEED_MEDIUM, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN));
			break;
		
		case GPIO_OUTPUT_SPEED_HIGH:
			IOPin->port->OSPEEDR |= setMask(GPIO_OUTPUT_SPEED_HIGH, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN);
			IOPin->port->OSPEEDR &= ~(clearMask(GPIO_OUTPUT_SPEED_HIGH, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN));
			break;
		
		case GPIO_OUTPUT_SPEED_VERYHIGH:
			IOPin->port->OSPEEDR |= setMask(GPIO_OUTPUT_SPEED_VERYHIGH, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN);
			IOPin->port->OSPEEDR &= ~(clearMask(GPIO_OUTPUT_SPEED_VERYHIGH, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN));
			break;
	}
}

static void set_pull(GPIO_Type *IOPin)
{
	switch(IOPin->pull)
	{
		
		case GPIO_PULL_NONE:
			IOPin->port->PUPDR |= setMask(GPIO_PULL_NONE, IOPin->pin, GPIO_PULL_SECTLEN);
			IOPin->port->PUPDR &= ~(clearMask(GPIO_PULL_NONE, IOPin->pin, GPIO_PULL_SECTLEN));
			break;
		
		case GPIO_PULL_UP:
			IOPin->port->PUPDR |= setMask(GPIO_PULL_UP, IOPin->pin, GPIO_PULL_SECTLEN);
			IOPin->port->PUPDR &= ~(clearMask(GPIO_PULL_UP, IOPin->pin, GPIO_PULL_SECTLEN));
			break;
		
		case GPIO_PULL_DOWN:
			IOPin->port->PUPDR |= setMask(GPIO_PULL_DOWN, IOPin->pin, GPIO_PULL_SECTLEN);
			IOPin->port->PUPDR &= ~(clearMask(GPIO_PULL_DOWN, IOPin->pin, GPIO_PULL_SECTLEN));
			break;
	}
}

static void set_altFunc(GPIO_Type *IOPin)
{
	//AFR HigherReg [1] or LowerReg [0] register 
	//		Default: LowerReg
	uint32_t AFR_Designator = 0;
	
	//Offset ConfigSector depending on HigherReg [1] or LowerReg [0]
	uint32_t UpLowRegCorrected_Pin = IOPin->pin;
	
	//HigherReg or LowerReg
	if(IOPin->pin >= 8)
	{
		AFR_Designator = 1;
		UpLowRegCorrected_Pin -= 8;		//offset HigherReg so SET and CLEAR formulas work.  8->0, 9->1, 10->2, etc...
		
	}
	
	IOPin->port->AFR[AFR_Designator] |= setMask(IOPin->altFunctionNum, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
	IOPin->port->AFR[AFR_Designator] &= ~(clearMask(IOPin->altFunctionNum, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
	
}
void gpio_init(GPIO_Type *IOPin)
{
	//Enable RCC for GPIO Port Peripheral
	enGPIOPortClock(IOPin->port);
	
	//set mode
	set_mode(IOPin);
	
	//set output_Type & output_Speed
	if ((IOPin->mode == GPIO_MODE_OUTPUT) || (IOPin->mode == GPIO_MODE_ALTFUNC))
	{
		set_outputType(IOPin);
		set_outputSpeed(IOPin);
	}
	
	//set pull
	set_pull(IOPin);
	
	//set altFunction
	if (IOPin->mode == GPIO_MODE_ALTFUNC)
	{
		set_altFunc(IOPin);
	}
}

void gpio_writePin(GPIO_Type *IOPin, uint32_t state)
{
	//Set Register
	if(state == 1)
	{
		IOPin->port->BSRR |= (1<<(IOPin->pin));
	}
	
	//Set Register
	if(state == 0)
	{
		IOPin->port->BSRR |= (1<<((IOPin->pin) + 16));
	}
}
