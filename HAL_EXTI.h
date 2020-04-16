#ifndef HAL_EXTI
#define HAL_EXTI

#include "stm32l432xx.h"
#include "HAL_SYSCFG.h"
#include "RegisterMaskConstructors.h"

//GPIO must be configured as input, output or alternate function mode
//to use EXTI

//EXTI Edge Triggers
#define EXTI_EDGE_RISING 0
#define EXTI_EDGE_FALLING 1
#define EXTI_EDGE_BOTH 2

//EXTI Modes
#define EXTI_MODE_INTERUPT 0
#define EXTI_MODE_EVENT 1

typedef struct
{
	uint32_t line;					//EXTI#
	uint32_t edgeTrigger;		//Rising, Falling, or Both
	uint32_t mode;					//Interupt or Event
} EXTI_Type;

static void enEXTI(EXTI_Type* EXTI_object);
void exti_setEdge(EXTI_Type* EXTI_object);
static void enRisingEdge(uint32_t EXTI_Line);
static void enFallingEdge(uint32_t EXTI_Line);

void exti_init(EXTI_Type* EXTI_object, IRQn_Type irqNumber);
void exti_clearPending(uint32_t EXTI_Line);

#endif //HAL_EXTI
