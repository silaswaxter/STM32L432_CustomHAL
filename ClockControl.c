#include "ClockControl.h"
#include <stdint.h>

void setSystemClock(SystemClock_T* SystemClockConfig)
{
	//Set MSI speed
	if(SystemClockConfig->SystemClockSource == ClockSource_MSI)
	{
		setMSIRANGE(SystemClockConfig->SYSCLK_MSISpeed);
	}
			
	//Enable System-ClockSource
	if(SystemClockConfig->SystemClockSource == ClockSource_PLL)
	{
		//Enable PLL
		enClock(SystemClockConfig->PLL_Configuration->PLLClockSource);
		
		//Config PLL
		setPLL(SystemClockConfig->PLL_Configuration);
	}
	else
	{
		//Enable ClockSource
		enClock(SystemClockConfig->SystemClockSource);
	}
	
	//Enable Flash Prefetch
	enFlashPrefetch();
	
	//Set Flash Wait Cycles (Flash R/W is slower than MCU speed)
	setFlashLatency(SystemClockConfig->TargetSystemClockSpeedMHZ);
	
	//Set SYSCLK SOURCE
	setSYSCLKSource(SystemClockConfig->SystemClockSource);
	
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
	
	if(clockSource == ClockSource_LSE)
	{
		//Disable RCC_BDCR Write Protection
		RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;		//Turn on PWR Reg via APB1
		PWR->CR1 |= PWR_CR1_DBP;								//Disable write protection
		
		//Turn on LSE
		RCC->BDCR |= RCC_BDCR_LSEON;						
		
		PWR->CR1 &= ~PWR_CR1_DBP_Msk;						//Enable write protection
		
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_PWREN_Msk;		//Turn off PWR Reg via APB1
		
		//Wait for HSE Stablization
		while(!(RCC->BDCR & RCC_BDCR_LSERDY));
	}
}

void disClock(ClockSource_T clockSource)
{
	if(clockSource == ClockSource_HSE)
	{
		//Disable HSE Bypass
		RCC->CR &= ~RCC_CR_HSEBYP_Msk;
		
		//Turn Off HSE
		RCC->CR &= ~RCC_CR_HSEON_Msk;
		
		//Wait for hardware confirmation
		while((RCC->CR & RCC_CR_HSERDY));
	}
	
	if(clockSource == ClockSource_HSI16)
	{
		//Turn Off HSI16
		RCC->CR &= ~RCC_CR_HSION_Msk;
		
		//Wait for hardware confirmation
		while((RCC->CR & RCC_CR_HSIRDY));
	}
	
	if(clockSource == ClockSource_MSI)
	{
		//Turn Off MSI
		RCC->CR &= ~RCC_CR_MSION_Msk;
		
		//Wait for hardware confirmation
		while((RCC->CR & RCC_CR_MSIRDY));
	}
}

void setMSIRANGE(MSISpeed_T speed)
{
	//Set Flash Latency
	enFlashPrefetch();
	if(speed == MSI_48000)
		setFlashLatency(48);
	
	if((speed == MSI_32000) || (speed == MSI_24000))
		setFlashLatency(32);
	
	else
		setFlashLatency(16);		//MSI_100 || MSI_200 || MSI_400 || MSI_800 || MSI_1000 || MSI_2000 || MSI_4000 || MSI_8000 || MSI_16000
	
	
	//Set MSIRANGE
	if(!(RCC->CR & RCC_CR_MSION) || ((RCC->CR & RCC_CR_MSION) && (RCC->CR & RCC_CR_MSIRDY)))	//If (MSI is OFF) or (MSI is ON and RDY)
	{	
		RCC->CR &= ~RCC_CR_MSIRANGE_Msk;		//Reset MSIRANGE
		RCC->CR |= (speed<<4);							//Set MSIRANGE
		
		RCC->CR |= RCC_CR_MSIRGSEL;			//[MUST BE SET AFTER MSIRANGE] MSIRANGE is from CR register
	}
	
	SystemCoreClockUpdate();			//If MSI is SYSCLK source
}

void setPLL(PLL_T* PLL_Config)
{
	//Enable PLL-ClockSource
	enClock(PLL_Config->PLLClockSource);
	
	if(PLL_Config->PLLClockSource == ClockSource_MSI)
	{
		//Set MSI Speed
		setMSIRANGE(PLL_Config->PLL_MSISpeed);
		
		//Enable LSE so PLL source can be MSI (LSE provides live calibration)
		enClock(ClockSource_LSE);
		
		//Allow MSI to be PLL source
		RCC->CR |= RCC_CR_MSIPLLEN;
	}
	
	//Disable PLL
	RCC->CR &= ~RCC_CR_PLLON_Msk;
	while((RCC->CR & RCC_CR_PLLRDY))				//Wait for PLL Disable
	;
	
	//Reset PLL
	RCC->PLLCFGR &= 0;
	
	//Set PLL source
	if(PLL_Config->PLLClockSource == ClockSource_HSE)
	{
		RCC->PLLCFGR |= (0x03);
	}
	if(PLL_Config->PLLClockSource == ClockSource_HSI16)
	{
		RCC->PLLCFGR |= (0x02);
	}
	if(PLL_Config->PLLClockSource == ClockSource_MSI)
	{
		RCC->PLLCFGR |= (0x01);
	}
	
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

static void setSYSCLKSource(ClockSource_T clockSource)
{
	RCC->CFGR &= ~RCC_CFGR_SW_Msk;			//Reset SysClk Source Reg
	
	if(clockSource == ClockSource_HSE)
	{
		RCC->CFGR |= RCC_CFGR_SW_HSE;
		while(!(RCC->CFGR & RCC_CFGR_SWS_HSE));
	}
	
	if(clockSource == ClockSource_HSI16)
	{
		RCC->CFGR |= RCC_CFGR_SW_HSI;
		while(!(RCC->CFGR & RCC_CFGR_SWS_HSI));
	}
	
	if(clockSource == ClockSource_MSI)
	{
		RCC->CFGR |= RCC_CFGR_SW_MSI;
		while((RCC->CFGR & RCC_CFGR_SWS_MSI));
	}
	
	if(clockSource == ClockSource_PLL)
	{
		RCC->CFGR |= RCC_CFGR_SW_PLL;
		while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
	}
}
ClockSource_T getSYSCLKSource(void)
{
	if((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_HSE)
		return ClockSource_HSE;
	
	if((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_HSI)
		return ClockSource_HSI16;
	
	if((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_MSI)
		return ClockSource_MSI;
	
	if((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_PLL)
		return ClockSource_PLL;
	
	//ELSE ERROR
		return ClockSource_NotAvailable;
}

ClockSource_T getPLLClockSource(void)
{
	if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC_Msk) == RCC_PLLCFGR_PLLSRC_HSE)
		return ClockSource_HSE;
	
	if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC_Msk) == RCC_PLLCFGR_PLLSRC_HSI)
		return ClockSource_HSI16;
	
	if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC_Msk) == RCC_PLLCFGR_PLLSRC_MSI)
		return ClockSource_MSI;
	
	if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC_Msk) == 0x00)
		return ClockSource_NotAvailable;
	
	//ELSE ERROR
}
