#include "ClockControl.h"
#include <stdint.h>

void configSystemClock(SystemClock_T* SystemClockConfig)
{
	//Set MSI speed -- [config msi before turning it on]
	//setMSIRANGE(&SystemClockConfig->MSISpeed);
	
	//Enable OSC
	if(SystemClockConfig->SystemClockSource == ClockSource_PLL)
	{
		switch(SystemClockConfig->PLL_Configuration->PLLClockSource)
		{
			case PLLClockSource_HSE:
				enClock(ClockSource_HSE);
				break;
			case PLLClockSource_HSI:
				enClock(ClockSource_HSI16);
				break;
			case PLLClockSource_MSI:
				enClock(ClockSource_MSI);
				break;
		}
	}
	enClock(SystemClockConfig->SystemClockSource);
	
	//Enable Flash Prefetch
	enFlashPrefetch;
	
	//Set Flash Wait Cycles (Flash R/W is slower than MCU speed)
	setFlashLatency(SystemClockConfig->TargetSystemClockSpeedMHZ);
	
	//Config PLL
	configPLL(SystemClockConfig->PLL_Configuration);
	
	//Set SYSCLK SOURCE
	setSystemClockSource(SystemClockConfig->SystemClockSource);
	
	//Config AMBA/HLCK
	RCC->CFGR &= ~(RCC_CFGR_HPRE_Msk | RCC_CFGR_PPRE1_Msk
								| RCC_CFGR_PPRE2_Msk);										//Reset AMBA Registers
	RCC->CFGR |= SystemClockConfig->AHBPrescaler;
	RCC->CFGR |= SystemClockConfig->APB1Prescaler;
	RCC->CFGR |= SystemClockConfig->APB2Prescaler;
	
	//Update SystemCoreClock for CMSIS Functions
	SystemCoreClockUpdate();	
}

void enClock(ClockSource_T clockSource)
{
	if(clockSource == ClockSource_HSE)
	{
		//Enable HSE Bypass
		RCC->CR |= RCC_CR_HSEBYP;
		
		//Turn on HSE
		RCC->CR |= RCC_CR_HSEON;
		
		//Wait for HSE Stablization
		while(!(RCC->CR & RCC_CR_HSERDY));
	}
	if(clockSource == ClockSource_HSI16)
	{
		//Turn on HSI16
		RCC->CR |= RCC_CR_HSION;
		
		//Wait for HSI Stablization
		while(!(RCC->CR & RCC_CR_HSIRDY));
	}
	if(clockSource == ClockSource_MSI)
	{
		//Turn on MSI
		RCC->CR |= RCC_CR_MSION;
		
		//Wait for HSE Stablization
		while(!(RCC->CR & RCC_CR_MSIRDY));
	}
}

void setMSIRANGE(MSISpeed_T speed)
{
		//Set MSIRANGE
		RCC->CR |= RCC_CR_MSIRGSEL;							//MSIRANGE is from CR register
		RCC->CR &= ~RCC_CR_MSIRANGE_Msk;				//Reset MSIRANGE
		RCC->CR |= (speed<<4);
}

static void setSystemClockSource(ClockSource_T source)
{
	RCC->CFGR &= ~RCC_CFGR_SW_Msk;			//Reset SysClk Source Reg
	
	if(source == ClockSource_HSE)
	{
		RCC->CFGR |= RCC_CFGR_SW_HSE;
		while(!(RCC->CFGR & RCC_CFGR_SWS_HSE));
	}
	
	if(source == ClockSource_HSI16)
	{
		RCC->CFGR |= RCC_CFGR_SW_HSI;
		while(!(RCC->CFGR & RCC_CFGR_SWS_HSI));
	}
	
	if(source == ClockSource_MSI)
	{
		RCC->CFGR |= RCC_CFGR_SW_MSI;
		while(!(RCC->CFGR & RCC_CFGR_SWS_MSI));
	}
	
	if(source == ClockSource_PLL)
	{
		RCC->CFGR |= RCC_CFGR_SW_PLL;
		while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
	}
}
