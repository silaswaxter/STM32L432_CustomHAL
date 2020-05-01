#ifndef FLASH_FUNCTIONS_H
#define FLASH_FUNCTIONS_H

#include "stm32l432xx.h"

#define enFlashPrefetch FLASH->ACR |= FLASH_ACR_PRFTEN;		//Optimization: Instruction Prefetch

void setFlashLatency(int tgtSysClockSpeedMHz);

#endif //FLASH_FUNCTIONS_H
