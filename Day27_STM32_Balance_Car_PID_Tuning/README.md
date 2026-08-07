# Day27 STM32 Balance Car PID Tuning

## 项目简介

这是 STM32 100Days 的 Day27 学习工程，目标是完成两轮平衡小车的传感器、姿态解算、电机驱动和直立环 PID 基础验证，并为后续整车站立调参建立一个可复现的工程基线。

当前版本保留了当天最后一次代码状态，没有重新设计驱动，也没有修改 GPIO、DMP、MPU、PWM 或 PID 功能逻辑。该版本仍处于直立环调参阶段，不代表小车已经能够稳定自主站立。

## Hardware

- MCU：STM32F103C8T6
- IMU：标注为 MPU6050 的兼容模块
  - I2C 地址：`0x68`
  - 实际 `WHO_AM_I`：`0x70`
  - 按 MPU6500 兼容器件处理
- Motor Driver：L298N
- Motor：两路直流减速电机
- Encoder：当前工程使用 TIM2 对一路编码器进行正交解码
- Debugger：J-Link（Keil 工程仍保留 J-Link 配置）
- Power：电机与控制板必须使用正确的稳压供电，并确保所有模块共地

## Wiring

### MPU IMU

| MPU pin | STM32 pin |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| SCL | PA4 |
| SDA | PA5 |

### L298N control

| L298N pin | STM32 pin |
| --- | --- |
| IN1 | PB6 |
| IN2 | PB7 |
| IN3 | PB8 |
| IN4 | PB9 |
| OUT1 / OUT2 | Left motor |
| OUT3 / OUT4 | Right motor |

ENA、ENB 需要保持使能。左右电机安装方向相反时，应通过电机端极性或对应控制逻辑确认两轮的实际前进方向一致。

### Encoder

| Encoder signal | STM32 pin |
| --- | --- |
| Left encoder A | PA0 |
| Left encoder B | PA1 |

### Power safety

- 12V 电池只能接入 L298N 的电机电源输入，不可直接接 STM32 的 5V 或 3.3V 引脚。
- STM32、MPU 和 L298N 必须共地。
- 调整杜邦线、面包板或电源线前必须先断电。
- 裸露金属端必须绝缘，避免碰到下载器、USB 外壳或其他电路。

## Software Environment

- Keil MDK-ARM 5
- ARM Compiler 5
- STM32F10x Standard Peripheral Library
- Target device：STM32F103C8
- Debug adapter：J-Link / Cortex-M SWD

## Project Structure

```text
Day27_STM32_Balance_Car_PID_Tuning
├── CORE                    # Cortex-M3 core and startup files
├── HARDWARE
│   ├── ENCODE              # Encoder interface
│   ├── LED                 # Status LED
│   ├── MOTOR               # Motor direction and PWM output
│   ├── MPU6050
│   │   ├── DMP             # InvenSense DMP driver
│   │   └── IIC             # MPU software I2C
│   ├── PID                 # Upright and speed PID code
│   ├── PWM                 # TIM4 PWM configuration
│   └── TIM                 # Control scheduler and timer ISR
├── STM32F10x_FWLib         # STM32 standard peripheral library
├── SYSTEM                  # Delay, system and USART support
├── USER
│   ├── main.c
│   ├── LED.uvprojx         # Keil project
│   └── LED.uvoptx          # Keil target/debug options
├── .gitignore
└── README.md
```

## Control Flow

- `main.c` 初始化串口、延时、LED、软件 I2C、MPU/DMP、TIM4 PWM、TIM2 编码器和 TIM1 控制定时器。
- MPU 初始化和 DMP 初始化失败时，程序进入错误保护，不会继续启动电机控制。
- TIM1 以 1 ms 为基础节拍运行控制任务。
- 姿态读取与直立控制按 5 ms 周期执行。
- 编码器速度按 10 ms 周期计算。
- 连续 50 ms 没有获得有效 DMP 姿态数据时停止电机。

## Current PID Snapshot

当前发布副本保存的是当天最后一次源代码状态：

```c
P = 750;
I = 0;
D = 0;
aim_pitch = 0.24f;
```

速度环在当前调试阶段被明确关闭：

```c
pidv = 0.0f;
go((int)pidp, 0);
```

因此当前电机输出只用于观察直立环方向和 P 参数响应。`P = 750` 是调试快照，不是推荐参数；实车重新上电前应悬空轮子并确认输出方向和安全限幅。

## Day27 Verification Record

当天曾通过 Keil Watch 验证：

- 机械直立时 `Pitch` 约为 `0.17° ~ 0.30°`，目标角暂定为 `0.24°`。
- 前倾时 `Pitch` 约为 `-20°`。
- 后倾时 `Pitch` 约为 `+22° ~ +25°`。
- 前后倾时 `pidp` 能够改变符号。
- TIM4 的 `CCR1/CCR3` 与 `CCR2/CCR4` 能随方向切换。
- 两台电机本体和 L298N 两个通道经过交叉测试可以工作。

发布整理后执行了一次 Keil 完整构建，结果为：

```text
0 Error(s), 4 Warning(s)
```

警告来自原有源码，发布整理没有修改功能代码。构建产物已经从 GitHub 副本中删除，首次重新编译时 Keil 会自动生成。

## Debug Notes

Day27 排查出的主要问题包括：

1. 面包板、STM32 排针和杜邦线接触松动会造成 MPU 掉电、PWM 消失、单侧电机不转或 PC13 慢闪。
2. MPU 供电恢复后，如果程序已经进入初始化失败死循环，需要重新按 Reset 才会再次初始化。
3. 两台电机的机械安装方向相反，拆装后必须重新确认电机极性和实际前进方向。
4. 只看 PID 参数数值不能判断输出大小；输出还与角度误差、PWM 限幅和电机死区有关。
5. 本次调试过程中曾发生 J-Link 与带电裸露线头短接。再次实验前应先确认下载器、STM32 的 5V/3.3V、NRST 和所有供电线路安全正常。

## Build and Use

1. 使用 Keil MDK 5 打开 `USER/LED.uvprojx`。
2. 确认目标器件为 `STM32F103C8`。
3. 确认 ARM Compiler 5 和 STM32F1 Device Support 已安装。
4. Build Target；Keil 会重新创建 `OBJ` 和编译产物。
5. 首次连接实车前先让轮子悬空，确认 MPU 数据、两轮方向、PWM 输出和倒地停机保护。

## Learning Summary

平衡小车调试需要分层确认：电源与接触、传感器数据、姿态方向、控制量符号、PWM 寄存器、驱动模块、电机极性，最后才是 PID 参数。任何一层不稳定，都会让调参现象失去参考价值。

下一阶段应在硬件连接可靠后完成直立环 P/D 调参，再恢复速度环 PI；不要在直立环尚未稳定时同时调两个控制环。
