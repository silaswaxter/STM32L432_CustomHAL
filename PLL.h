#ifndef PLL
#define PLL

#include "stm32l432xx.h"

typedef enum {
	PLLClockSource_MSI = (0x01),
	PLLClockSource_HSI = (0x02),
	PLLClockSource_HSE = (0x03)
} PLLClockSource_T; 

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
	PLLClockSource_T PLLClockSource;
	PLLM_T PLLM;
	int PLLN;						//8 <= PLLN <= 86
	PLLR_T PLLR;
	
} PLL_T;

void configPLL(PLL_T* PLL_Config);

#endif //PLL
