# " Cấu hình ngắt ngoài cho 1 nút nhấn. Khi nhấn nút, trạng thái của LED sẽ đảo ngược. Trong quá trình đó 1 LED khác vẫn nhấp nháy với chu kỳ 1Hz"

- Các bước thực hiện 
## 1. Cấu hình GPIO VÀ NÚT BẤM 
- Cấu hình  chân PA0 và PA2  ở chế độ Output Push Pull, tốc độ xung nhịp 50MHz. Chân PA1 được cấu hình ở chế độ Input Up.
```c
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

```
## 2. cấu hình ngắt ngoài sẽ bao gồm NVIC và EXTI
```c
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
```
## 3. Hàm xử lý ngắt biến ledstate được gọi để chuyển trạng thái trong led 
```c
int ledstate = 0;
void EXTI1_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line1) != RESET){
            delay(100);
	    ledstate = !ledstate;
	    GPIO_WriteBit(GPIOA,GPIO_Pin_0,ledstate);
    }
       EXTI_ClearITPendingBit(EXTI_Line1);
}
``` 
- biến ledstate để lưu trạng thái trước đấy của led và sau khi ấn nút thì sẽ lấy giá trị đảo của biến ledstate.

#### 4. hàm main 
- trong hàm main sẽ gọi hàm config() và hàm ngatngoai() , trong while(1) thì led PA2 sẽ sáng nhấp nháy với 1 giây. Khi ta ấn nút bấm thì led PA1 sẽ chuyển trạng thái 
```c
int main(){
	config();
	ngatngoai();

	while(1){
		GPIO_Setbit(GPIOA,GPIO_Pin)
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		delay(1000);
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
		delay(1000);
	}
}
```


