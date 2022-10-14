---
title: Workflow của thư viện
author: TrungND
date: 2022-06-30
category: Jekyll
layout: post
---

## Luồng sử dụng thư viện

Trước khi bắt đầu thì cần phải khởi tạo sẵn object i2c instance của HAL (`I2CHandleTypedef`). Object này sẽ được dùng để feed cho thư viện.

Ngoài ra cũng phải có thông tin về địa chỉ I2C của chip. Chắc sẽ là 0x27 thôi. Nếu vậy thì giá trị này được lưu sẵn trong macro `PCF8574_I2C_ADDRESS` trong file header. Lưu ý là HAL I2C sử dụng địa chỉ 8 bit, tức là mình sẽ phải tự dịch trái địa chỉ 7 bit, HAL sẽ không tự động dịch. 

Đầu tiên là phải khởi tạo object LCD đã.

## Khởi tạo

Trong header có struct `LCDHandleType`, struct này chứa một vài thông tin liên quan tới thiết bị đang giao tiếp. Khởi tạo struct này để làm chỗ lưu thông tin đã.

Sau đó dùng `LCD_init` để thêm các giá trị mặc định vào struct. Thế là đã xong bước khởi tạo

## Sử dụng

Tới đây là đã có thể thoải mái sử dụng các hàm điều khiển, hiển thị cho LCD. Tuy nhiên phải lưu ý một vài điểm dưới đây:

1. Thư viện không tự xuống dòng. Do đó nếu số kí tự viết vào dòng vượt quá 20 thì phải chia nhỏ đoạn string ra và xuống dòng thủ công bằng hàm `LCD_moveCursor`


Tạm thế!!!