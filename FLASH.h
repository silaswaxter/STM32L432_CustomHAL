#ifndef FLASH_H
#define FLASH_H

#include "stm32l432xx.h"

void enFlashPrefetch(void);							//Optimization: Instruction Prefetch
void setFlashLatency(int tgtSysClockSpeedMHz);

#endif //FLASH_H
