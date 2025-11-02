#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include <string.h>
#include <stdio.h>


void Config_GPIO(){
	GPIO_InitTypeDef uart;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	uart.GPIO_Mode  = GPIO_Mode_AF_PP;
	uart.GPIO_Pin   = GPIO_Pin_9;     // TX
	uart.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &uart);
	
	uart.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	uart.GPIO_Pin   = GPIO_Pin_10;    // RX
	GPIO_Init(GPIOA, &uart);
}

void Config_Uart(){
	USART_InitTypeDef usart;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	usart.USART_BaudRate = 9600;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Tx;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &usart);
	USART_Cmd(USART1, ENABLE);
}

void uart_SendChar(char c){
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, c);
}

void uart_SendStr(char *str){
	while(*str != '\0'){
		uart_SendChar(*str++);
	}
}

void Delay_ms( int t){
	 int i, j;
	for(i = 0; i < t; i++){
		for(j = 0; j < 0x2aff; j++);
	}
}

int main(){
	int i;
	Config_GPIO();
	Config_Uart();

	uart_SendStr("Hello PTIT....\n");
	Delay_ms(1000);
	for(i = 0; i<5000000;i++);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_ClearFlag(PWR_FLAG_WU | PWR_FLAG_SB);
	PWR_EnterSTANDBYMode();  

	while(1){
	}
}
