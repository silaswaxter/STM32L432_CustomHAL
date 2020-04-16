#ifndef FLASHFUNCTIONS
#define FLASHFUNCTIONS

#include "stm32l432xx.h"

#define enFlashPrefetch FLASH->ACR |= FLASH_ACR_PRFTEN;		//Optimization: Instruction Prefetch

void setFlashLatency(int tgtSysClockSpeedMHz);

#endif //FLASHFUNCTIONS
