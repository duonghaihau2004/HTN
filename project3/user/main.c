#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "misc.h"

void delay(int time){
	int i,j;
	for(i = 0; i<time;i++){
		for(j =0; j<0x2aff;j++);
	}
}


void config(){
	GPIO_InitTypeDef gpio;
	GPIO_InitTypeDef button;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
		
	button.GPIO_Mode = GPIO_Mode_IPU;
	button.GPIO_Pin = GPIO_Pin_1;
	button.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&button);
	}

void ngatngoai(){
	NVIC_InitTypeDef nvic;
	EXTI_InitTypeDef exti;
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	nvic.NVIC_IRQChannel = EXTI1_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1); 
	exti.EXTI_Line = EXTI_Line1;
	exti.EXTI_LineCmd = ENABLE;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Falling; 
	EXTI_Init(&exti);
}

int ledstate = 0;
void EXTI1_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line1) != RESET){
        delay(100);
				ledstate = !ledstate;
				if(ledstate){
					GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				}
				else{
					GPIO_SetBits(GPIOA,GPIO_Pin_0);
				}
    }
		EXTI_ClearITPendingBit(EXTI_Line1);
}


int main(){
	config();
	ngatngoai();
	while(1){
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		delay(1000);
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
		delay(1000);
	}
}
