#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

void delay(int time){
	int i,j;
	for(i = 0; i<time;i++){
		for(j =0; j<0x2aff;j++);
	}
}


void config(){
	GPIO_InitTypeDef gpio;
		RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA,ENABLE);
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = GPIO_Pin_0;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
}
int main(){
	config();
	while(1){
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
		delay(200);
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
		delay(200);
	}
	return 0;
}
