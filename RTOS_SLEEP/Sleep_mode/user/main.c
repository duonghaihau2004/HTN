#include "stm32f10x.h"

void GPIO_Config(void);
void TIM2_Config(void);

int main(void)
{
		int i;
    GPIO_Config();
    TIM2_Config();
		for(i = 0; i<5000000;i++);
    while (1)
    {
        __WFI();   
    }
}

void GPIO_Config(void)
{
		GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    gpio.GPIO_Pin = GPIO_Pin_0;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gpio);
}

void TIM2_Config(void)
{
	
	  TimBaseInitTypeDef tim;
		NVIC_InitTypeDef nvic;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    tim.TIM_Prescaler = 7200 - 1;   
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_Period = 5000 - 1;    
    tim.TIM_ClockDivision = 0;
    tim.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &tim);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);

    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIOA->ODR ^= GPIO_Pin_0;  // Toggle LED
    }
}
