#ifndef HAL_SYSCFG
#define HAL_SYSCFG

#include "stm32l432xx.h"
#include "RegisterMaskConstructors.h"

#define SYSCFG_EXTI_SECTLEN 4					//sector length is technically 3, but used 4 so standard register pattern can be used
static uint32_t SYSCFG_ClockEnabled(void);
static void enSYSCFG_Clock(void);

void enEXTI_GPIO(GPIO_TypeDef *GPIO_Port, uint32_t GPIO_Pin);
				
#endif //HAL_SYSCFG
