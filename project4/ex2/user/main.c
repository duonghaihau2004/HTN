#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void config_led(void);
void timer_init(void);

void config_led(){
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_0;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);
}

void timer_init(void){ 
    TIM_TimeBaseInitTypeDef timer_init; 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    
    // ARR = 5000-1 => 500 ms
    timer_init.TIM_CounterMode = TIM_CounterMode_Up; 
    timer_init.TIM_Period = 5000 - 1; 
    timer_init.TIM_ClockDivision = 0; 
    timer_init.TIM_RepetitionCounter = 0; 
    timer_init.TIM_Prescaler = 7200 - 1;  
    
    TIM_TimeBaseInit(TIM2,&timer_init); 
    TIM_ClearFlag(TIM2,TIM_FLAG_Update); 
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 
    NVIC_EnableIRQ(TIM2_IRQn);  
    TIM_Cmd(TIM2,ENABLE); 
}

// ISR
int ledstate = 0;
void TIM2_IRQHandler(){ 
    if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET){   
        ledstate = !ledstate;
				if(ledstate){
					GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				}
				else GPIO_SetBits(GPIOA,GPIO_Pin_0);
    } 
	 TIM_ClearITPendingBit(TIM2,TIM_IT_Update); 
}

int main(void){
    config_led();
    timer_init();
    while(1){
			
    }
}




