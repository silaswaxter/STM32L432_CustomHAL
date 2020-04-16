#include "HAL_EXTI.h"

static void enEXTI(EXTI_Type* EXTI_object)
{
	//Register Set 1
	if(EXTI_object->line <= 31)
	{
		EXTI->IMR1 |= (1<<EXTI_object->line);
	}
	
	//Register Set 2
	if(EXTI_object->line >= 32)
	{
		EXTI->IMR2 |= (1<<(EXTI_object->line - 32));
	}
}

static void enRisingEdge(uint32_t EXTI_Line)
{
	//Register Set 1
	if(EXTI_Line <= 31)
	{
		EXTI->RTSR1 |= (1<<EXTI_Line);
	}
	
	//Register Set 2
	if(EXTI_Line >= 32)
	{
		EXTI->RTSR2 |= (1<<(EXTI_Line - 32));
	}
}

static void enFallingEdge(uint32_t EXTI_Line)
{
	//Register Set 1
	if(EXTI_Line <= 31)
	{
		EXTI->FTSR1 |= (1<<EXTI_Line);
	}
	
	//Register Set 2
	if(EXTI_Line >= 32)
	{
		EXTI->FTSR2 |= (1<<(EXTI_Line - 32));
	}
}


void exti_init(EXTI_Type* EXTI_object, IRQn_Type irqNumber)
{
	enEXTI(EXTI_object);
	exti_setEdge(EXTI_object);
	
	NVIC_EnableIRQ(irqNumber);
}

void exti_clearPending(uint32_t EXTI_Line)
{
	//Register Set 1
	if(EXTI_Line <= 31)
	{
		EXTI->PR1 |= (1<<EXTI_Line);
	}
	
	//Register Set 2
	if(EXTI_Line >= 32)
	{
		EXTI->PR2 |= (1<<(EXTI_Line - 32));
	}
}

void exti_setEdge(EXTI_Type* EXTI_object)
{
	if(EXTI_object->edgeTrigger == EXTI_EDGE_RISING)
	{
		enRisingEdge(EXTI_object->line);
	}
	
	if(EXTI_object->edgeTrigger == EXTI_EDGE_FALLING)
	{
		enFallingEdge(EXTI_object->line);
	}
	
	if(EXTI_object->edgeTrigger == EXTI_EDGE_BOTH)
	{
		enRisingEdge(EXTI_object->line);
		enFallingEdge(EXTI_object->line);
	}
}
