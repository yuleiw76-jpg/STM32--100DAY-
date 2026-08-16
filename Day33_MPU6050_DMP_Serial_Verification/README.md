# Day33_MPU6050_DMP_Serial_Verification

## 1. 今日目标

今天使用新购买的真正 MPU6050 模块，重新完成 MPU6050、DMP 和串口输出验证，为后续平衡小车直立环 PID 调参排除传感器数据不稳定的问题。

主要目标：

- 确认 I2C 通信是否正常；
- 读取 `WHO_AM_I` 并确认芯片身份；
- 验证 MPU6050 初始化；
- 验证 DMP 固件加载与 FIFO 姿态数据读取；
- 学习 USB 转 TTL 串口模块的基本使用；
- 通过串口连续观察 Pitch、Roll、Yaw。

## 2. Hardware

- MCU：STM32F103C8T6
- IMU：MPU6050
- 调试器：J-Link
- 串口模块：CH340 USB 转 TTL
- 供电电压：MPU6050 使用 STM32 的 3.3V

## 3. 接线

### MPU6050

| MPU6050 | STM32F103C8T6 |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| SCL | PA4 |
| SDA | PA5 |

### USB 转 TTL

| USB 转 TTL | STM32F103C8T6 |
| --- | --- |
| RX | PA9（USART1 TX） |
| TX | PA10（USART1 RX，可选） |
| GND | GND |

> 串口收发需要交叉连接：模块 RX 接 STM32 TX，模块 TX 接 STM32 RX。STM32 已有独立供电时，不使用 USB 转 TTL 的 VCC 给主板重复供电。

## 4. 串口设置

- 串口：COM3（以电脑实际识别结果为准）
- 波特率：115200
- 数据位：8
- 校验位：None
- 停止位：1
- 流控：None
- 显示编码：ASCII

这组参数通常记作 `115200 8N1`。

## 5. 实验过程

1. 从原 MPU6050 教程工程创建独立验证副本，不直接修改平衡小车工程。
2. 使用 PA4、PA5 软件 I2C 连接新 MPU6050。
3. 在初始化前直接读取 `WHO_AM_I` 寄存器。
4. 判断 I2C 返回值和芯片身份，不再无条件认为初始化成功。
5. 依次执行 MPU6050 初始化和 DMP 初始化。
6. 通过 USART1 将初始化状态与 Pitch、Roll、Yaw 连续发送到串口助手。
7. 保持模块静止并改变姿态，观察数据的稳定性和变化方向。

## 6. 验证结果

串口启动信息：

```text
MPU6050 DMP VERIFY START
I2C_READ=0, WHO_AM_I=0x68
MPU INIT OK
DMP INIT OK, 200HZ
```

持续输出示例：

```text
Pitch=0.21, Roll=0.86, Yaw=0.07
Pitch=0.36, Roll=0.84, Yaw=0.14
Pitch=0.50, Roll=0.83, Yaw=0.20
```

最终确认：

- `I2C_READ=0`：软件 I2C 读取成功；
- `WHO_AM_I=0x68`：模块为真正的 MPU6050；
- `MPU INIT OK`：基础寄存器初始化成功；
- `DMP INIT OK, 200HZ`：DMP 固件与 FIFO 配置成功；
- Pitch、Roll、Yaw 可以连续输出，并且静止状态明显比旧模块稳定。

## 7. 与旧模块的区别

此前使用的模块 `WHO_AM_I` 返回 `0x70`，实际更接近 MPU6500 兼容芯片。虽然经过兼容修改后能够运行，但曾出现初始化时间长、偶尔初始化失败、Pitch 异常跳变或静止时保持在错误角度等问题。

新模块返回标准的 `0x68`，使用 MPU6050 分支即可完成初始化，姿态数据稳定性也明显改善。这说明之前平衡小车难以调参不一定只是 PID 参数问题，传感器型号和数据可靠性同样是关键前提。

## 8. 工程说明

Keil 工程文件：

```text
Code/MPU6050_DMP_Serial_Verification/USER/Template.uvprojx
```

主要文件：

- `USER/main.c`：验证流程、错误保护和串口输出；
- `HARDWARE/MPU6050/MPU6050.c`：MPU6050 与 DMP 初始化、姿态读取；
- `HARDWARE/MPU6050/IIC/`：PA4、PA5 软件 I2C；
- `HARDWARE/MPU6050/DMP/`：DMP 驱动；
- `SYSTEM/usart/`：USART1 串口驱动。

Keil 5 ARMCC 构建结果：

```text
0 Error(s), 0 Warning(s)
```

## 9. Learning Summary

今天第一次完整使用 USB 转 TTL 和串口助手，理解了 TX/RX 需要交叉连接，也掌握了 `115200 8N1` 的含义。相比只看 LED 或 OLED，串口能够同时显示初始化阶段、错误代码和实时姿态数据，调试效率更高。

本次实验也进一步说明：嵌入式调试必须先验证数据源，再进入控制算法。只有 I2C、芯片身份、DMP 初始化和姿态数据都可靠，PID 调参才有意义。

## 10. 下一步

明天继续使用之前已经运行过的平衡小车工程，不采用未经验证的新框架。先创建副本，再把传感器配置从旧模块的 MPU6500/`0x70` 切换为今天验证成功的 MPU6050/`0x68`，电机、编码器和 PID 结构暂时保持不变。
