#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* Task prototypes */
void Task_LED1(void *pvParameters);
void Task_LED2(void *pvParameters);
void Task_LED3(void *pvParameters);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* T?o task b?ng FreeRTOS g?c */
  xTaskCreate(Task_LED1, "LED1", 128, NULL, 1, NULL); // priority 1
  xTaskCreate(Task_LED2, "LED2", 128, NULL, 2, NULL); // priority 2
  xTaskCreate(Task_LED3, "LED3", 128, NULL, 3, NULL); // priority 3

  /* B?t d?u scheduler */
  vTaskStartScheduler();

  /* N?u code d?n dây thì l?i: không d? RAM heap */
  while (1);
}

/* Task 1: LED PA0, chu k? 333ms */
void Task_LED1(void *pvParameters)
{
  const TickType_t delayTime = pdMS_TO_TICKS(333);
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
    vTaskDelay(delayTime);
  }
}

/* Task 2: LED PA1, chu k? 100ms */
void Task_LED2(void *pvParameters)
{
  const TickType_t delayTime = pdMS_TO_TICKS(100);
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
    vTaskDelay(delayTime);
  }
}

/* Task 3: LED PA2, chu k? 40ms */
void Task_LED3(void *pvParameters)
{
  const TickType_t delayTime = pdMS_TO_TICKS(40);
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
    vTaskDelay(delayTime);
  }
}

/* C?u hình GPIO */
static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* N?u scheduler d?ng do l?i RAM */
void vApplicationMallocFailedHook(void)
{
  while (1);
}

/* N?u task l?i stack overflow */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
  (void)xTask;
  (void)pcTaskName;
  while (1);
}
