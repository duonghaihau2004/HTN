#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework
#include "string.h"
void config(void);
void usart(void);
void usart_SenChar(char c);

void config(){
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
}

void usart(){
	USART_InitTypeDef uart;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	uart.USART_BaudRate = 9600;
	uart.USART_Mode = USART_Mode_Tx;
	uart.USART_Parity = USART_Parity_No;
	uart.USART_WordLength = USART_WordLength_8b;
	uart.USART_StopBits = USART_StopBits_1 ;
	uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&uart);
	USART_Cmd(USART1, ENABLE); 
}


void uart_SendChar(char c){ 
 USART_SendData(USART1,c); 
	
 while(USART_GetFlagStatus(USART1, 
USART_FLAG_TXE)==RESET); 
} 

void uart_SendStr(char *Str){
	while(*Str != NULL){
		uart_SendChar(*Str++);
	}
}
void delay_ms(int time){
	int i,j;
	for(i =0; i<time;i++){
		for(j = 0; j<0x2aff;j++);
	}
}


int main(){
	config();
	usart();
	while(1){
		uart_SendStr("HELLO From STM32!\n");
		delay_ms(500);
	}
}