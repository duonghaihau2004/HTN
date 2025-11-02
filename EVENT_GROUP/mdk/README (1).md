# Bài 14.1: Sử dụng Event Group để điều khiển LED

## Giới thiệu

Chương trình này sử dụng vi điều khiển **STM32F103C8T6**, minh họa cách dùng **Event Group** trong **FreeRTOS** để đồng bộ hóa nhiều **Task**.

Mục tiêu:

- Tạo 4 **Task** hoạt động song song:
  1. `Main_Task` – phát tín hiệu sự kiện cho từng LED.
  2. `LED1_Task`, `LED2_Task`, `LED3_Task` – nhận sự kiện và điều khiển LED tương ứng.
- Sử dụng `Event Group` để truyền tín hiệu giữa các `Task`

---

## Cấu hình phần cứng

| Thành phần | Chân GPIO | Chế độ | Mô tả |
|-------------|------------|--------|--------|
| LED1 | PA0 | Output push-pull | Nháy khi nhận BIT_LED1 |
| LED2 | PA1 | Output push-pull | Nháy khi nhận BIT_LED2 |
| LED3 | PA2 | Output push-pull | Nháy khi nhận BIT_LED3 |

---

## Nguyên lý hoạt động

### 1. **Cấu hình GPIO**
- Bật clock cho `GPIOA`.
- Cấu hình các chân `PA0`, `PA1`, `PA2` làm ngõ ra LED.

### 2. **Khởi tạo Event Group**
```c
xEventGroup = xEventGroupCreate();
```
- Tạo một `Event Group` dùng để quản lý các cờ sự kiện.
- 3 sự kiện được định nghĩa:
  ```c
  #define BIT_LED1   (1 << 0)
  #define BIT_LED2   (1 << 1)
  #define BIT_LED3   (1 << 2)
  ```
### 3. **Các Task chính**
 1. Main Task
  ```c
  void Main_Task(void *argument)
{
    while(1)
    {
        xEventGroupSetBits(xEventGroup, BIT_LED1);
        vTaskDelay(pdMS_TO_TICKS(1000));

        xEventGroupSetBits(xEventGroup, BIT_LED2);
        vTaskDelay(pdMS_TO_TICKS(1000));

        xEventGroupSetBits(xEventGroup, BIT_LED3);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
  ```
- Cứ mỗi 1 giây, task này gửi một sự kiện mới 1s mỗi LED
- Quá trình lặp lại liên tục.
  
2. LED Task
```c
uxBits = xEventGroupWaitBits(xEventGroup, BIT_LED1, pdTRUE, pdFALSE, portMAX_DELAY);
if(uxBits & BIT_LED1)
{
    GPIOA->ODR ^= GPIO_Pin_0;
    vTaskDelay(pdMS_TO_TICKS(300));
}
```
- Mỗi task sẽ chờ đến khi bit tương ứng được set.
- Khi nhận được bit đảo trạng thái LED `(GPIOA->ODR ^= GPIO_Pin_x`), delay 300ms để LED nhấp nháy.

## Video mô phỏng
