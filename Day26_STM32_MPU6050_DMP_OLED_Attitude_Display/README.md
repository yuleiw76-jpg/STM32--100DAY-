# STM32F103 MPU6050 DMP OLED Attitude Display

## 项目简介

使用 STM32F103 驱动 MPU6050 兼容 IMU，通过 DMP 算法计算姿态角，并使用 OLED 实时显示 Pitch、Roll、Yaw。

本项目是 STM32 100Days 的 Day26 最终成功版本。OLED 与 IMU 分别使用独立的软件 I2C，总线和驱动逻辑互不合并。

## Hardware

MCU：

- STM32F103

IMU：

- MPU6050 模块
- 实际 `WHO_AM_I` 返回 `0x70`，按 MPU6500 兼容器件配置
- I2C 从机地址仍为 `0x68`

OLED：

- 0.96 英寸 I2C OLED

下载与调试：

- J-Link

## 接线

OLED：

| OLED | STM32F103 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SCL | PB6 |
| SDA | PB7 |

MPU：

| MPU6050 模块 | STM32F103 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SCL | PA4 |
| SDA | PA5 |
| AD0 | GND |

## Features

- Software I2C
- MPU 初始化与 `WHO_AM_I` 检查
- DMP 固件加载与校验
- FIFO 数据读取
- 四元数姿态解算
- OLED 实时显示 Pitch、Roll、Yaw
- OLED 显示分阶段初始化和错误状态

## Debug 记录

### 1. OLED 黑屏

原因：

- 面包板和 STM32 引脚接触不良

处理：

- 使用 GPIO 诊断波形和逻辑分析仪检查引脚
- 重新确认供电、共地和面包板连接

### 2. I2C 通信异常

修复内容：

- 使用开漏输出
- 检查地址、寄存器和数据阶段的 ACK
- 统一底层读写函数返回值
- 修正 START、STOP、ACK 和读取采样时序
- 总线异常时执行恢复时钟和 STOP

### 3. WHO_AM_I 异常

实测结果：

```text
I2C 地址:  0x68
WHO_AM_I: 0x70
```

虽然模块标称 MPU6050，但实际寄存器行为兼容 MPU6500，因此工程启用了 InvenSense 驱动中的 MPU6500 配置。`0x68` 是 I2C 地址，`0x70` 是身份寄存器返回值，两者不能混淆。

### 4. DMP 调试

最终结果：

- DMP 固件成功写入并校验
- DMP 输出 6 轴四元数
- FIFO 读取正常
- 输出频率为 20Hz
- OLED 实时显示 Pitch、Roll、Yaw

## Learning Summary

嵌入式调试不能只看代码，需要结合：

- 硬件连接和供电
- 逻辑分析仪波形
- I2C START、ACK、STOP 和重复起始协议
- 设备地址与身份寄存器数据
- 分阶段初始化和错误返回值
- DMP 固件、FIFO、四元数和欧拉角之间的关系

调试时应先验证物理连接，再验证总线时序和 ACK，最后进入驱动和算法层。模块丝印只能作为参考，最终应以真实寄存器读数为依据。

## Keil 使用方法

1. 使用 Keil MDK-ARM 5 打开 `USER/Template.uvprojx`。
2. 选择目标 `MPU6050_OLED_DMP`。
3. 执行 Build/Rebuild。
4. 连接 J-Link 后下载到 STM32F103。

仓库不包含 `OBJ`、AXF、HEX、MAP、Listing 和本机 Keil 缓存。首次编译时 Keil 会重新生成构建目录和固件文件。
