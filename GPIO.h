#ifndef GPIO_H
#define GPIO_H

#include "stm32l432xx.h"

//Standard GPIO Mask Schema
#define setMask( tgtSect, sectIndex, sectLen) 	(tgtSect<<(sectIndex*sectLen))
#define clearMask( tgtSect, sectIndex, sectLen)	((tgtSect ^ ((1<<sectLen) - 1))<<(sectLen*sectIndex))

//Logic Levels
#define HIGH 1
#define LOW 0			
				
//GPIO MODES
#define GPIO_MODE_INPUT (0x00)
#define GPIO_MODE_OUTPUT (0x01)
#define GPIO_MODE_ALTFUNC (0x02)
#define GPIO_MODE_ANALOG (0x03)

#define GPIO_MODE_SECTLEN 2

//GPIO OUTPUT_TYPES
#define GPIO_OUTPUT_TYPE_PUSH (0x00)
#define GPIO_OUTPUT_TYPE_DRAIN (0x01)

#define GPIO_OUTPUT_TYPE_SECTLEN 1

//GPIO OUTPUT_SPEED
#define GPIO_OUTPUT_SPEED_LOW (0x00)
#define GPIO_OUTPUT_SPEED_MEDIUM (0x01)
#define GPIO_OUTPUT_SPEED_HIGH (0x02)
#define GPIO_OUTPUT_SPEED_VERYHIGH (0x03)

#define GPIO_OUTPUT_SPEED_SECTLEN 2

//GPIO PULL
#define GPIO_PULL_NONE (0x00)
#define GPIO_PULL_UP (0x01)
#define GPIO_PULL_DOWN (0x02)

#define GPIO_PULL_SECTLEN 2

//GPIO ALT_FUNCTION
#define GPIO_ALTFUNCTION_SECTLEN 4

typedef struct
{
	GPIO_TypeDef *port;		
	uint32_t pin;
	uint32_t mode;
	uint32_t outputType; 		//push-pull vs open-drain
	uint32_t outputSpeed;
	uint32_t pull;						//pull-up/pull-down resistors
	uint32_t altFunctionNum;
} GPIO_Type;

static void enGPIOPortClock(GPIO_Type *GPIOConfig);
static void setMode(GPIO_Type *GPIOConfig);
static void setOutputType(GPIO_Type *GPIOConfig);
static void setOutputSpeed(GPIO_Type *GPIOConfig);
static void setPull(GPIO_Type *GPIOConfig);
static void setAltFunction(GPIO_Type *GPIOConfig);

void gpioInit(GPIO_Type *GPIOConfig);
void gpioWrite(GPIO_Type *GPIOConfig, uint32_t state);

#endif //GPIO_H
