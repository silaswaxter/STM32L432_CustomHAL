#include "PLL.h"
#include <stdint.h>

void configPLL(PLL_T* PLL_Config){
	//Disable PLL
	RCC->CR &= ~RCC_CR_PLLON_Msk;
	while((RCC->CR & RCC_CR_PLLRDY))				//Wait for PLL Disable
	;
	
	//Reset PLL
	RCC->PLLCFGR &= 0;
	
	//Set PLL source
	RCC->PLLCFGR |= PLL_Config->PLLClockSource;
	
	//Set PLLM, PLLN, PLLR
	RCC->PLLCFGR |= (PLL_Config->PLLM<<4);
	RCC->PLLCFGR |= (PLL_Config->PLLN<<8);
	RCC->PLLCFGR |= (PLL_Config->PLLR<<25);
	
	//Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY));				//Wait for PLL Enable
	
	//Enable PLL Output
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;			//Enable PLL Output from PLLR (Last Manipulator)
}
