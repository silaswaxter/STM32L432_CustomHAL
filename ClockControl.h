#ifndef CLOCKCONTROL
#define CLOCKCONTROL

#include "stm32l432xx.h"
#include "PLL.h"
#include "FlashFunctions.h"

typedef enum {
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


typedef enum {
	ClockSource_MSI,
	ClockSource_HSI16,
	ClockSource_HSE,
	ClockSource_PLL
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


typedef struct SystemClock_Type {
	int TargetSystemClockSpeedMHZ;					//Used for # of Flash wait cycles
	
	PLL_T* PLL_Configuration;
	
	MSISpeed_T MSISpeed;
	
	ClockSource_T SystemClockSource;
	
	APBPrescaler_T APB1Prescaler;
	APBPrescaler_T APB2Prescaler;
	AHBPrescaler_T AHBPrescaler;
} SystemClock_T;

void configSystemClock(SystemClock_T* SystemClockConfig);
void enClock(ClockSource_T clockSource);
void setMSIRANGE(MSISpeed_T speed);
static void setSystemClockSource(ClockSource_T source);

#endif //ClockControl
