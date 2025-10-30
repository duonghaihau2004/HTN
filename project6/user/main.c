#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "string.h"
#include "stdio.h"

#define BH1750_ADDR 0x23

void Delay_ms(unsigned int t){
	unsigned int i, j;
	for(i = 0; i< t; i++){
		for(j=0; j< 0x2aff; j++);
	}
}

void Config_USART(){
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	gpio.GPIO_Mode			= GPIO_Mode_AF_PP;
	gpio.GPIO_Pin				= GPIO_Pin_9;
	gpio.GPIO_Speed			= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	
	gpio.GPIO_Mode			= GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Pin				= GPIO_Pin_10;
	gpio.GPIO_Speed			= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	GPIO_Init(GPIOA, &gpio);
	
	usart.USART_BaudRate				= 9600;
	usart.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	usart.USART_Mode						= USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_Parity					= USART_Parity_No;
	usart.USART_StopBits				= USART_StopBits_1;
	usart.USART_WordLength			= USART_WordLength_8b;
	
	USART_Init(USART1, &usart);
	USART_Cmd(USART1, ENABLE);
}

void USARTx_SendChar(char c){
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, c);
}

void USARTx_SendString(char *str){
	while(*str){
		USARTx_SendChar(*str++);
	}
}

struct __FILE { 
    int dummy; 
}; 
 
FILE __stdout; 
  
int fputc(int ch, FILE *f) { 
    USARTx_SendChar(ch); 
    return ch; 
}

void Config_I2C(){
	GPIO_InitTypeDef gpio_i2c;
	I2C_InitTypeDef	i2c;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	gpio_i2c.GPIO_Mode		= GPIO_Mode_AF_OD;
	gpio_i2c.GPIO_Pin			= GPIO_Pin_6 | GPIO_Pin_7;
	gpio_i2c.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_i2c);
	
	i2c.I2C_Ack						= I2C_Ack_Enable;
	i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c.I2C_ClockSpeed		= 100000;
	i2c.I2C_DutyCycle			= I2C_DutyCycle_2;
	i2c.I2C_Mode 					= I2C_Mode_I2C;
	i2c.I2C_OwnAddress1		= 0x00;
	
	I2C_Init(I2C1, &i2c);
	I2C_Cmd(I2C1, ENABLE);
}

/*================= BH1750 =================*/
void BH1750_Init(void)
{
		// bat dau giao tiep 
    I2C_GenerateSTART(I2C1, ENABLE); // bao hieu 1 giao tiep moi chuan bi bat dau 
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  
		
		// gui dia chi va che do ghi 
    I2C_Send7bitAddress(I2C1, BH1750_ADDR<<1, I2C_Direction_Transmitter); // gui dia chi 7 bit cung bit 0 cuoi de chi dinh che do ghi
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));// cho xac nhan no da nhan dia chi
	 // gui cac lenh dieu khien 
    I2C_SendData(I2C1, 0x01); // Power ON
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		// ket thuc 1 giao dich	
    I2C_GenerateSTOP(I2C1, ENABLE);
		// bat dau 1 giao dich moi
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
    I2C_Send7bitAddress(I2C1, BH1750_ADDR<<1, I2C_Direction_Transmitter); // gui dia chi 7 buit ket hop bit 0 cuoi de chi dinh che do ghi
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); // cho xsc nhan no da nhan dia chi
		
    I2C_SendData(I2C1, 0x07); // Reset
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
		// bat dau giao dich moi
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, BH1750_ADDR<<1, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, 0x10);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
}

uint16_t BH1750_ReadLight(void)
{
    uint16_t value;
    uint8_t high, low;

    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, BH1750_ADDR<<1, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); // cho den khi master chinh thuc vao che do nhan du lieu
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));// cho den khi co 1 byte du lieu san sang de doc tu slave
    high = I2C_ReceiveData(I2C1);

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    low = I2C_ReceiveData(I2C1);

    I2C_AcknowledgeConfig(I2C1, DISABLE); 
    I2C_GenerateSTOP(I2C1, ENABLE);
		// vo hieu hoa ack va tao tin hieu stop de ket thuc viec doc du lieu 
    I2C_AcknowledgeConfig(I2C1, ENABLE);
		
    value = (high<<8) | low;
		// ket hop hai byte dã doc de tao thành mot giá tri 16-bit. Phép dich bit << 8 dua byte cao ve dúng vi trí.
    return (uint16_t)(value / 1.2);
}


int main(){
	Config_USART();
	Config_I2C();
	BH1750_Init();
	while(1){
		uint16_t lux = BH1750_ReadLight();
    printf( "Lux: %u lx\r\n", lux);
		Delay_ms(500);
	}
	
}