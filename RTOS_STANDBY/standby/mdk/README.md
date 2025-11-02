# Bài 14.3: Dùng UART gửi kí tự và đo điện năng ở chế độ Standby

## Giới thiệu

Chương trình này được viết cho **STM32F103C8T6**, minh họa **truyền dữ liệu UART** và chuyển vi điều khiển sang chế độ **Standby** tiết kiệm năng lượng. 

Mục tiêu:
- Cấu hình USART để truyền dữ liệu nối tiếp.
- Sau khi gửi xong, chuyển sang chế độ Standby.
- Khi ở Standby, tiến hành đo dòng điện tiêu thụ
  
---

## Cấu hình phần cứng

| Thành phần | Chân GPIO | Chế độ | Mô tả |
|-------------|------------|--------|--------|
| UART1_TX | PA9 | Push-pull | Truyền dữ liệu |
| UART1_RX | PA10 | Input Floating | Nhận dữ liệu |

---

## Nguyên lý hoạt động

### 1. **Khởi tạo GPIO**
- Bật clock cho cổng GPIOA.
- Cấu hình **PA9** là ngõ ra **Alternate Function Push-Pull** (TX).
- Cấu hình **PA10** là Input Floating (RX).

### 2. **Cấu hình UART**
   - Baudrate: 9600, dữ liệu: 8 bit.
   - Stop bit: 1, no parity.

### 3. **Truyền dữ liệu UART**
- Gửi từng ký tự bằng hàm `uart_SendChar()`.
- Gửi chuỗi `"Hello PTIT....\r\n"` bằng `uart_SendStr()`.
- Gửi tín hiệu Semaphore

### 4. **Chuyển sang chế độ Standby**
   ```c
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_ClearFlag(PWR_FLAG_WU | PWR_FLAG_SB);
	PWR_EnterSTANDBYMode();
   ```  
- Sau khi gửi dữ liệu, chương trình bật clock cho **PWR** và **BKP**.
- Xóa cờ Wakeup và Standby bằng `PWR_ClearFlag()`.
- Gọi `PWR_EnterSTANDBYMode()` để đưa STM32 vào **chế độ Standby**.
  
## Video mô phỏng
> https://www.youtube.com/watch?v=voNrLphzZOE
