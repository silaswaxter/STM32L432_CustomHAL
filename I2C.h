#ifndef I2C_H
#define I2C_H

#include "stm32l432xx.h"

typedef enum {
	i2cSpeedMode_100khz = 1,
	i2cSpeedMode_400khz = 2,
	i2cSpeedMode_1000khz = 3,
} i2cSpeedMode_T;

static uint32_t i2cTimingReg[] = {
	0x2000090E, 0x0000020B, 0x00000001, 		//8Mhz
	0x00303D5B, 0x0010061A, 0x00000107,			//16Mhz
	0x00506682, 0x00200C28, 0x0010030D,			//24Mhz
	0x00707CBB, 0x00300F38, 0x00100413,			//32Mhz
	0x20303E5D, 0x2010091A, 0x20000209,			//48Mhz
	0x10606DA4, 0x00501D63, 0x00200923,			//56Mhz
	0x10707DBC, 0x00602173, 0x00300B29,			//64Mhz
	0x10808DD3, 0x00702681, 0x00300D2E,			//72Mhz
	0x10909CEC, 0x00702991, 0x00300F33,			//80Mhz
};

typedef struct {
	I2C_TypeDef* i2cPeriph;
	uint32_t i2cNum;
	i2cSpeedMode_T speedMode;				//Timing
	uint32_t dmaEnabled_Tx;
	uint32_t dmaEnabled_Rx;
	uint32_t autoendEnabled;	
} I2C_T;

static void enI2CClock(uint32_t i2cNum);
static void getI2CTimingRegVal(i2cSpeedMode_T speedMode);
inline static void setI2CSpeedMode(I2C_T* I2CConfig);

void i2cInit(I2C_T* I2CConfig);

#endif //I2C_H
