# Bài 14.2: Đo điện năng tiêu thụ ở chế độ Sleep

## Giới thiệu:

Chương trình này được viết cho **STM32F103C8T6**, minh họa cách sử dụng Timer2  để tạo ngắt định kỳ và cho CPU vào chế độ ngủ giữa các lần ngắt.

---

## Các bước thực hiện:
### 1. Hàm main()
```c
int main(void)
{
    GPIO_Config();
    TIM2_Config();
    while (1)
    {
        __WFI(); // Chờ ngắt (tiết kiệm năng lượng)
    }
}
```
- Gọi cấu hình GPIO và Timer2
- Sau khi cấu hình, CPU vào lệnh `__WFI()` (Wait For Interrupt) → CPU **ngủ**, chỉ thức dậy khi có ngắt từ Timer2.

### 2. Cấu hình chân GPIO.

```c
void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    gpio.GPIO_Pin = GPIO_Pin_0;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gpio);
}
```
- Cấu hình PA0 để điều khiển led.
  
### 3. Cấu hình Timer.
```c
void TIM2_Config(void)
{
    TIM_TimeBaseInitTypeDef tim;
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
}
```
- Kích hoạt clock cho TIM2.  
- Thiết lập bộ chia tần số (`Prescaler = 72 - 1`) để tạo xung clock 1 MHz (1 tick = 1 µs).  
- Chu kỳ đếm (Period) = 5000 → 0.5 giây → Timer2 sinh ra ngắt mỗi 0.5 giây. 
- Bộ đếm chạy **chế độ lên (Upcounter)**. 

 ### 4. Cấu hình NVIC (Ngắt)
 ```c
nvic.NVIC_IRQChannel = TIM2_IRQn;
nvic.NVIC_IRQChannelPreemptionPriority = 0;
nvic.NVIC_IRQChannelSubPriority = 0;
nvic.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&nvic);
}
```
- Kích hoạt ngắt TIM2_IRQn trong bộ NVIC.
- Ưu tiên cao nhất

### 5. Hàm phục vụ ngắt (ISR)
```c
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIOA->ODR ^= GPIO_Pin_0; 
    }
}
```
- Xóa cờ ngắt bằng `TIM_ClearITPendingBit`.
- Đảo bit PA0 → LED bật/tắt luân phiên.

## Video Mô Phỏng:
> https://www.youtube.com/shorts/JZyqGIBSkZw






