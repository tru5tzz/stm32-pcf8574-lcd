### int new_LCD_object(LCDHandleType *lcd, uint8_t address, I2C_HandleTypeDef *i2c_instant)

Trả về giá trị int (không cần quan tâm lắm đâu).

Đối số:

1. LCDHandleType *: nhớ là trong hàm main thì nên khởi tạo trước, khởi tạo ấy, đừng chỉ khai báo con trỏ.
2. uint8_t address: địa chỉ module i2c, được lưu mặc định trong macro `PCF8574_I2C_ADDRESS` là `0x27`. Nhớ shift trái 1 bit rồi mới truyền vào đây.

    ```C
    PCF8574_I2C_ADDRESS << 1
    ```

3. I2C_HandleTypeDef *: CubeMX tự khởi tạo hộ mình cái này rồi, chỉ việc truyền vào thôi.
