#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

void config_led(void);
void timer_init(void);
void delay_1ms(void);
void delay_ms(int time);

void config_led(){
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = GPIO_Pin_0;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
}

void timer_init(){
	TIM_TimeBaseInitTypeDef timer;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	timer.TIM_ClockDivision = 0;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = 36000 - 1;
	timer.TIM_Prescaler = 2 -1 ;
	timer.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&timer);
}

void delay_1ms(){
	timer_init();
	TIM_Cmd(TIM2,ENABLE);
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2) < 36000);
	TIM_Cmd(TIM2,DISABLE);
}

void delay_ms(int time){
	while(time--){
		delay_1ms();
	}
}

int main(){
	config_led();
	timer_init();
	while(1){
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
		delay_ms(500);
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
		delay_ms(500);
	}
}