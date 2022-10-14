# Thư viện giao tiếp STM32 với LCD 20x4 I2C (PCF8574)

Có gì thiếu sót thì góp ý bổ sung.

Hướng dẫn chi tiết có tại ....

## Limitations

1. Không tự xuống dòng, nếu số kí tự lớn hơn 20 (1 dòng) thì phải di chuyển thủ công con trỏ xuống dòng tiếp theo
2. Không nhớ là STM32 HAL dùng địa chỉ 7 bit hay 8 bit, hình như là 8 bit. Nếu là 8 bit thì khi khởi tạo đối tượng LCD thì phải tự dịch 1 bit sang bên trái
