#include "stm32l4xx.h"
#include "HAL_GPIO.h"
#include "HAL_SYSCFG.h"
#include "HAL_EXTI.h"
#include "ClockControl.h"

GPIO_Type led3;
EXTI_Type extiB1;
void LEDInteruptInit(void);
void SystemClockInit(void);

int main()
{	
	SystemClockInit();		//Set SYSCLK @ 80MHz
	
	
	
	
	LEDInteruptInit();
	
	while(1)
	{ 
		
	}
}

void EXTI1_IRQHandler()
{
	//Clear Pending Interupt
	exti_clearPending(extiB1.line);
	
	//Turn On LED
	gpio_writePin(&led3, HIGH);
}

void SystemClockInit(void)
{
	PLL_T PLLTest;
	PLLTest.PLLClockSource = ClockSource_HSE;
	//PLLTest.PLL_MSISpeed = MSI_8000;
	PLLTest.PLLM = PLLM_1;
	PLLTest.PLLN = 20;
	PLLTest.PLLR = PLLR_2;
	
	SystemClock_T HSEPLL80;
	HSEPLL80.AHBPrescaler = AHB_1;
	HSEPLL80.APB1Prescaler = APB_1;
	HSEPLL80.APB2Prescaler = APB_1;
	HSEPLL80.PLL_Configuration = &PLLTest;
	HSEPLL80.SystemClockSource = ClockSource_PLL;
	//HSEPLL80.SYSCLK_MSISpeed = MSI_8000;
	HSEPLL80.TargetSystemClockSpeedMHZ = 8;
	
	setSystemClock(&HSEPLL80);
}

void LEDInteruptInit(void)
{
	//Led GPIO-Pin Setup
	led3.port = GPIOB;
	led3.pin = 3;
	led3.mode = GPIO_MODE_OUTPUT;
	led3.output_Type = GPIO_OUTPUT_TYPE_PUSH;
	led3.output_Speed = GPIO_OUTPUT_SPEED_MEDIUM;
	led3.pull = GPIO_PULL_NONE;
	
	gpio_init(&led3);
	
	//Interupt GPIO-Pin Setup
	GPIO_Type interuptGPIO;
	interuptGPIO.port = GPIOB;
	interuptGPIO.pin = 1;
	interuptGPIO.mode = GPIO_MODE_OUTPUT;
	interuptGPIO.output_Type = GPIO_OUTPUT_TYPE_PUSH;
	interuptGPIO.output_Speed = GPIO_OUTPUT_SPEED_MEDIUM;
	interuptGPIO.pull = GPIO_PULL_NONE;
	
	gpio_init(&interuptGPIO);

	//EXTI External Interupt Setup
	extiB1.line = 1;
	extiB1.edgeTrigger = EXTI_EDGE_RISING;
	extiB1.mode = EXTI_MODE_INTERUPT;

	//Interupt Initialization
	enEXTI_GPIO(interuptGPIO.port, interuptGPIO.pin);
	
	//EXTI Initialization
	exti_init(&extiB1, EXTI1_IRQn);
}
