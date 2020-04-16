#include "FlashFunctions.h"
#include <stdint.h>

void setFlashLatency(int tgtSysClockSpeedMHz)
{
	FLASH->ACR &= ~FLASH_ACR_LATENCY_Msk;			//RESET Latency Reg
	
	if(tgtSysClockSpeedMHz <= 16)
	{
		FLASH->ACR |= FLASH_ACR_LATENCY_0WS;
	}
	if(tgtSysClockSpeedMHz <= 32)
	{
		FLASH->ACR |= FLASH_ACR_LATENCY_1WS;
	}
	if(tgtSysClockSpeedMHz <= 48)
	{
		FLASH->ACR |= FLASH_ACR_LATENCY_2WS;
	}
	if(tgtSysClockSpeedMHz <= 64)
	{
		FLASH->ACR |= FLASH_ACR_LATENCY_3WS;
	}
	if(tgtSysClockSpeedMHz <= 80)
	{
		FLASH->ACR |= FLASH_ACR_LATENCY_4WS;
	}
}

