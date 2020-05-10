#include "I2C.h"

static void enI2CClock(uint32_t i2cNum)
{
	if(i2cNum == 1)
		RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;
	
	if(i2cNum == 2)
		RCC->APB1ENR1 |= RCC_APB1ENR1_I2C3EN;
}

static void getI2CTimingRegVal(i2cSpeedMode_T speedMode)
{
	//getPLCLK1
		//SYSCLOCK
		//AHBPrescaler
		//APB1Prescaler
}

inline static void setI2CSpeedMode(I2C_T* I2CConfig)
{
	//I2CConfig->i2cPeriph->TIMINGR = 
}

void i2cInit(I2C_T* I2CConfig)
{
	//Enable Periph Clock
	enI2CClock(I2CConfig->i2cNum);
	
	
}
