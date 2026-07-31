# Day23 I2C OLED Protocol Analysis

## 今日目标

使用逻辑分析仪分析 STM32 与 OLED 模块之间的 I2C 通信过程。

通过 Saleae Logic Analyzer 观察：

- I2C START 信号
- I2C STOP 信号
- 地址传输
- ACK 应答
- OLED 命令与数据区别

理解代码执行与底层通信波形之间的对应关系。

---

# 1. I2C通信复习

I2C总线包含两根信号线：

- SCL：时钟线
- SDA：数据线


通信流程：


START
↓
Slave Address
↓
ACK
↓
Control Byte
↓
Command/Data
↓
ACK
↓
STOP


---

# 2. OLED通信分析

OLED使用SSD1306驱动。

设备地址：


0x3C


---

## OLED命令模式

发送：


0x00


表示：

Command Mode

即后续数据作为OLED控制命令。


例如：


0xAE


表示：

Display OFF

关闭OLED显示。


通信：


START

0x3C + ACK

0x00 + ACK

0xAE + ACK

STOP


---

# 3. OLED数据模式

显示字符时：

发送控制字节：


0x40


表示：

Data Mode


后续发送的数据用于更新OLED显存。

---

# 4. Saleae Logic Analyzer使用

步骤：

1. 连接SCL、SDA信号

2. 设置采样率


24MHz


3. 开始采集

4. 添加I2C Analyzer


配置：


SCL Channel 0

SDA Channel 1


---

# 5. START与STOP识别

START:

当：


SCL保持高电平

SDA:
1 -> 0


表示开始通信。


STOP:

当：


SCL保持高电平

SDA:
0 -> 1


表示通信结束。

---

# 6. ACK理解

I2C每发送8bit数据后：

第9个时钟周期用于ACK。


发送方释放SDA：

- ACK时，从机拉低SDA
- NACK时，保持高电平


注意：

释放SDA不一定一定看到拉高。

如果从机立即拉低应答，SDA可能一直保持低电平。

---

# 7. 实验总结

通过逻辑分析仪观察OLED通信：

成功验证：

- OLED初始化命令传输
- I2C地址通信
- ACK应答机制
- Command/Data模式区别


理解：

代码中的OLED函数最终都会转换为实际的I2C电平变化。


## 收获

第一次将软件代码与硬件波形对应起来。

从观察LED现象，进入分析底层通信协议阶段。