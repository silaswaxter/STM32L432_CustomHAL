//To use EXTI, GPIO_MODE_TYPE = Input, Output, or AF

#ifndef EXTI_H
#define EXTI_H

#include "stm32l432xx.h"
#include "GPIO.h"

//SYSCFG EXTI Sector
#define SYSCFG_EXTI_SECTLEN 4					//sector length is technically 3, but used 4 so standard register pattern can be used

//EXTI Edge Triggers
#define EXTI_EDGE_RISING 0b01
#define EXTI_EDGE_FALLING 0b10
#define EXTI_EDGE_BOTH 0b11

//EXTI Modes
#define EXTI_MODE_INTERUPT 0
#define EXTI_MODE_EVENT 1

typedef struct
{
	uint32_t line;					//EXTI#
	uint32_t edgeTrigger;		//Rising, Falling, or Both
	uint32_t mode;					//Interupt or Event
} EXTI_Type;

static void enEXTIForSYSCFG(GPIO_Type* GPIOConfig);
static void setInteruptMask(EXTI_Type* EXTIConfig);
static void enRisingEdge(uint32_t EXTI_Line);
static void enFallingEdge(uint32_t EXTI_Line);

void extiInit(GPIO_Type* GPIOConfig, EXTI_Type* EXTIConfig, IRQn_Type irqNumber);
void extiSetEdge(EXTI_Type* EXTIConfig);
void extiClearPending(uint32_t EXTI_Line);

#endif //EXTI_H
