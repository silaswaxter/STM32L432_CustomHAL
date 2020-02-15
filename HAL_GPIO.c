#include "HAL_GPIO.h"
#include <stdint.h>

static void set_mode(GPIO_Type *IOPin)
{
	switch (IOPin->mode)
	{
		case GPIO_MODE_INPUT:
			IOPin->port->MODER |= SET_MASK(GPIO_MODE_INPUT, IOPin->pin, GPIO_MODE_SECTLEN);
			IOPin->port->MODER &= ~(CLEAR_MASK(GPIO_MODE_INPUT, IOPin->pin, GPIO_MODE_SECTLEN));
			break;
		
		case GPIO_MODE_OUTPUT:
			IOPin->port->MODER |= SET_MASK(GPIO_MODE_OUTPUT, IOPin->pin, GPIO_MODE_SECTLEN);
			IOPin->port->MODER &= ~(CLEAR_MASK(GPIO_MODE_OUTPUT, IOPin->pin, GPIO_MODE_SECTLEN));
			break;
		
		case GPIO_MODE_ALTFUNC:
			IOPin->port->MODER |= SET_MASK(GPIO_MODE_ALTFUNC, IOPin->pin, GPIO_MODE_SECTLEN);
			IOPin->port->MODER &= ~(CLEAR_MASK(GPIO_MODE_ALTFUNC, IOPin->pin, GPIO_MODE_SECTLEN));
			break;
		
		case GPIO_MODE_ANALOG:
			IOPin->port->MODER |= SET_MASK(GPIO_MODE_ANALOG, IOPin->pin, GPIO_MODE_SECTLEN);
			IOPin->port->MODER &= ~(CLEAR_MASK(GPIO_MODE_ANALOG, IOPin->pin, GPIO_MODE_SECTLEN));
			break;
	}
}

static void set_outputType(GPIO_Type *IOPin)
{
	switch (IOPin->output_Type)
	{
		case GPIO_OUTPUT_TYPE_PUSH:
			IOPin->port->MODER |= SET_MASK(GPIO_OUTPUT_TYPE_PUSH, IOPin->pin, GPIO_OUTPUT_TYPE_SECTLEN);
			IOPin->port->MODER &= ~(CLEAR_MASK(GPIO_OUTPUT_TYPE_PUSH, IOPin->pin, GPIO_OUTPUT_TYPE_SECTLEN));
			break;
		
		case GPIO_OUTPUT_TYPE_DRAIN:
			IOPin->port->MODER |= SET_MASK(GPIO_OUTPUT_TYPE_DRAIN, IOPin->pin, GPIO_OUTPUT_TYPE_SECTLEN);
			IOPin->port->MODER &= ~(CLEAR_MASK(GPIO_OUTPUT_TYPE_DRAIN, IOPin->pin, GPIO_OUTPUT_TYPE_SECTLEN));
			break;
	}
}

static void set_outputSpeed(GPIO_Type *IOPin)
{
	switch (IOPin->output_Speed)
	{
		case GPIO_OUTPUT_SPEED_LOW:
			IOPin->port->OSPEEDR |= SET_MASK(GPIO_OUTPUT_SPEED_LOW, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN);
			IOPin->port->OSPEEDR &= ~(CLEAR_MASK(GPIO_OUTPUT_SPEED_LOW, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN));
			break;
		
		case GPIO_OUTPUT_SPEED_MEDIUM:
			IOPin->port->OSPEEDR |= SET_MASK(GPIO_OUTPUT_SPEED_MEDIUM, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN);
			IOPin->port->OSPEEDR &= ~(CLEAR_MASK(GPIO_OUTPUT_SPEED_MEDIUM, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN));
			break;
		
		case GPIO_OUTPUT_SPEED_HIGH:
			IOPin->port->OSPEEDR |= SET_MASK(GPIO_OUTPUT_SPEED_HIGH, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN);
			IOPin->port->OSPEEDR &= ~(CLEAR_MASK(GPIO_OUTPUT_SPEED_HIGH, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN));
			break;
		
		case GPIO_OUTPUT_SPEED_VERYHIGH:
			IOPin->port->OSPEEDR |= SET_MASK(GPIO_OUTPUT_SPEED_VERYHIGH, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN);
			IOPin->port->OSPEEDR &= ~(CLEAR_MASK(GPIO_OUTPUT_SPEED_VERYHIGH, IOPin->pin, GPIO_OUTPUT_SPEED_SECTLEN));
			break;
	}
}

static void set_pull(GPIO_Type *IOPin)
{
	switch(IOPin->pull)
	{
		
		case GPIO_PULL_NONE:
			IOPin->port->PUPDR |= SET_MASK(GPIO_PULL_NONE, IOPin->pin, GPIO_PULL_SECTLEN);
			IOPin->port->PUPDR &= ~(CLEAR_MASK(GPIO_PULL_NONE, IOPin->pin, GPIO_PULL_SECTLEN));
			break;
		
		case GPIO_PULL_UP:
			IOPin->port->PUPDR |= SET_MASK(GPIO_PULL_UP, IOPin->pin, GPIO_PULL_SECTLEN);
			IOPin->port->PUPDR &= ~(CLEAR_MASK(GPIO_PULL_UP, IOPin->pin, GPIO_PULL_SECTLEN));
			break;
		
		case GPIO_PULL_DOWN:
			IOPin->port->PUPDR |= SET_MASK(GPIO_PULL_DOWN, IOPin->pin, GPIO_PULL_SECTLEN);
			IOPin->port->PUPDR &= ~(CLEAR_MASK(GPIO_PULL_DOWN, IOPin->pin, GPIO_PULL_SECTLEN));
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
	
	switch(IOPin->altFunction)
	{
		case GPIO_ALTFUNC_AF0:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF0, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF0, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF1:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF1, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF1, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF2:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF2, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF2, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF3:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF3, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF3, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF4:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF4, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF4, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF5:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF5, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF5, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF6:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF6, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF6, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF7:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF7, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF7, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF8:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF8, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF8, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF9:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF9, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF9, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF10:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF10, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF10, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF11:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF11, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF11, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF12:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF12, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF12, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF13:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF13, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF13, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF14:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF14, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF14, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
		
		case GPIO_ALTFUNC_AF15:
			IOPin->port->AFR[AFR_Designator] |= SET_MASK(GPIO_ALTFUNC_AF15, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN);
			IOPin->port->AFR[AFR_Designator] &= ~(CLEAR_MASK(GPIO_ALTFUNC_AF15, UpLowRegCorrected_Pin, GPIO_ALTFUNC_SECTLEN));
			break;
	}
}


void initPin(GPIO_Type *IOPin)													//Must be called before configPin()
{
	if (IOPin->port == GPIOA)
	{
		GPIO_ENABLE_PORTA;
	}
	
	if (IOPin->port == GPIOB)
	{
		GPIO_ENABLE_PORTB;
	}
	
	if (IOPin->port == GPIOC)
	{
		GPIO_ENABLE_PORTC;
	}
	
	/*
	if (GPIO_PIN->port == GPIOD)
	{
		GPIO_ENABLE_PORTD;
	}
	
	if (GPIO_PIN->port == GPIOE)
	{
		GPIO_ENABLE_PORTE;
	}
	
	if (GPIO_PIN->port == GPIOH)
	{
		GPIO_ENABLE_PORTH;
	}
	*/
	
}

void configPin(GPIO_Type *IOPin)												//called AFTER initPin()
{
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

void writePin(GPIO_Type *IOPin, uint32_t state)
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
