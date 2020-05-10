#ifndef CLOCK_CONTROL_H
#define CLOCK_CONTROL_H

#include "stm32l432xx.h"
#include "FLASH.h"

typedef enum {							//values mapped to MSIRANGE bit config
	MSI_100 = (0x00),
	MSI_200 = (0x01),
	MSI_400 = (0x02),
	MSI_800 = (0x03),
	MSI_1000 = (0x04),
	MSI_2000 = (0x05),
	MSI_4000 = (0x06),
	MSI_8000 = (0x07),
	MSI_16000 = (0x08),
	MSI_24000 = (0x09),
	MSI_32000 = (0x0A),
	MSI_48000 = (0x0B)
} MSISpeed_T;

typedef enum {							//Random defined values
	ClockSource_MSI = (0x01),
	ClockSource_HSI16 = (0x02),
	ClockSource_HSE = (0x04),
	ClockSource_PLL = (0x08),
	ClockSource_LSE = (0xFF),
	ClockSource_NotAvailable = (0x10)
} ClockSource_T; 

//AMBA
typedef enum {
	AHB_1 = (0x00),
	AHB_2 = (0x08),
	AHB_4 = (0x09),
	AHB_8 = (0x0A),
	AHB_16 = (0x0B),
	AHB_64 = (0x0C),
	AHB_128 = (0x0D),
	AHB_256 = (0x0E),
	AHB_512 = (0x0F)	
} AHBPrescaler_T;

typedef enum {
	APB_1 = (0x00),
	APB_2 = (0x04),
	APB_4 = (0x05),
	APB_8 = (0x06),
	APB_16 = (0x07)
} APBPrescaler_T;

//PLL
typedef enum {
	PLLM_1 = (0x00),
	PLLM_2 = (0x01),
	PLLM_3 = (0x02),
	PLLM_4 = (0x03),
	PLLM_5 = (0x04),
	PLLM_6 = (0x05),
	PLLM_7 = (0x06),
	PLLM_8 = (0x07)
} PLLM_T;

typedef enum {
	PLLR_2 = (0x00),
	PLLR_4 = (0x01),
	PLLR_6 = (0x02),
	PLLR_8 = (0x03)
} PLLR_T;

typedef struct {
	ClockSource_T PLLClockSource;
	PLLM_T PLLM;
	int PLLN;						//8 <= PLLN <= 86
	PLLR_T PLLR;
	
	MSISpeed_T PLL_MSISpeed;
	
} PLL_T;

typedef struct SystemClock_Type {
	int TargetSystemClockSpeedMHZ;					//Used for # of Flash wait cycles
	
	PLL_T* PLL_Configuration;
	
	MSISpeed_T SYSCLK_MSISpeed;							//Used only when MSI is directly SYSCLK source (non-PLL)
	
	ClockSource_T SystemClockSource;
	
	APBPrescaler_T APB1Prescaler;
	APBPrescaler_T APB2Prescaler;
	AHBPrescaler_T AHBPrescaler;						//Determines SystemCoreClock=HCLK
} SystemClock_T;

void setSystemClock(SystemClock_T* SystemClockConfig);
void enClock(ClockSource_T clockSource);
void setMSIRANGE(MSISpeed_T speed);
void setPLL(PLL_T* PLL_Config); 
static void setSYSCLKSource(ClockSource_T clockSource);
ClockSource_T getSYSCLKSource(void);
ClockSource_T getPLLClockSource(void);
uint32_t getAPBCLK(uint32_t APBCLKNum);				//PCLKx

#endif //CLOCK_CONTROL_H
