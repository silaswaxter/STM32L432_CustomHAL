#include "stm32l4xx.h"
#include "HAL_GPIO.h"

int main()
{
	GPIO_Type led3;
	led3.port = GPIOB;
	led3.pin = 3;
	led3.mode = GPIO_MODE_OUTPUT;
	led3.output_Type = GPIO_OUTPUT_TYPE_PUSH;
	led3.output_Speed = GPIO_OUTPUT_SPEED_MEDIUM;
	led3.pull = GPIO_PULL_NONE;
	
	initPin(&led3);
	configPin(&led3);
	
	while(1)
	{
		for(int i = 0; i < 500000; i++);
		writePin(&led3, HIGH);
		for(int i = 0; i < 500000; i++);
		writePin(&led3, LOW);
	}
}
