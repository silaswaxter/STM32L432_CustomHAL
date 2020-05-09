#ifndef HAL_SYSCFG_H
#define HAL_SYSCFG_H

#include "stm32l432xx.h"

//Standard EXTI Mask Schema
#define setMask( tgtSect, sectIndex, sectLen) 	(tgtSect<<(sectIndex*sectLen))
#define clearMask( tgtSect, sectIndex, sectLen)	((tgtSect ^ ((1<<sectLen) - 1))<<(sectLen*sectIndex))



#define SYSCFG_EXTI_SECTLEN 4					//sector length is technically 3, but used 4 so standard register pattern can be used
static uint32_t SYSCFG_ClockEnabled(void);
static void enSYSCFG_Clock(void);

void enEXTI_GPIO(GPIO_TypeDef *GPIO_Port, uint32_t GPIO_Pin);

#endif //HAL_SYSCFG_H
