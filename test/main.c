#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core

void config(void);

TaskHandle_t Task01Handle;
TaskHandle_t Task02Handle;
TaskHandle_t Task03Handle;

void Task01Funct(void * argument);
void Task02Funct(void * argument);
void Task03Funct(void * argument);

int main(void)
{
	config();
	xTaskCreate(Task01Funct, "LED3Hz", 128, NULL, 1, &Task01Handle);
	xTaskCreate(Task02Funct, "LED10Hz", 128, NULL, 1, &Task02Handle);
	xTaskCreate(Task03Funct, "LED25Hz", 128, NULL, 1, &Task03Handle);

	vTaskStartScheduler();
  while (1)
  {
		
  }
}
void config(){
		GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
}


void Task01Funct(void * argument)
{
  for(;;)
  {
		GPIOA -> ODR ^= GPIO_Pin_0;
		vTaskDelay(167); // 3Hz
  }
}

void Task02Funct(void * argument)
{
  for(;;)
  {
		GPIOA -> ODR ^= GPIO_Pin_1;
		vTaskDelay(50); //10Hz
  }
}

void Task03Funct(void * argument)
{
  for(;;)
  {
		GPIOA -> ODR ^= GPIO_Pin_2;
		vTaskDelay(20); /// 25Hz
  }
}

