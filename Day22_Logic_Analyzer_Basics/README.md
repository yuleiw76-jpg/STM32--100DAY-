# Day22：逻辑分析仪基础

## 项目名称

Day22_Logic_Analyzer_Basics

## 学习日期

2026-07-29（STM32 100 天学习计划第 22 天）

## 学习内容

- 理解从 GPIO 寄存器、引脚电平到数字波形的完整关系。
- 使用逻辑分析仪观察 STM32 GPIO 输出的高低电平和持续时间。
- 通过修改 `delay_ms()` 参数，验证代码延时与实测波形之间的对应关系。
- 熟悉 Keil、J-Link 与逻辑分析仪组成的基础调试流程。

## 实验目的

让 STM32F103C8 的 PC13 引脚周期性输出数字电平，并使用逻辑分析仪测量波形，确认程序设置的延时时间能够正确反映到硬件引脚上。

## 硬件环境

- STM32F103C8 开发板
- 板载 PC13 LED
- USB 逻辑分析仪及测试线
- J-Link 下载/调试器
- USB 供电与连接线

逻辑分析仪需要与开发板共地，信号通道连接 PC13。采样输入不得超过逻辑分析仪允许的电压范围。

## 软件环境

- Keil MDK-ARM / µVision 5
- ARM Compiler 5（工程使用 ARM-ADS 工具链配置）
- Keil STM32F1xx Device Family Pack 2.2.0
- STM32F10x Standard Peripheral Library
- 与所用逻辑分析仪匹配的上位机软件

## 实验现象

程序运行后，PC13 持续输出周期波形：

- 低电平约 1000 ms
- 高电平约 500 ms
- 完整周期约 1500 ms

在逻辑分析仪中可以直接测量高、低电平持续时间。PC13 板载 LED 通常为低电平点亮，因此会呈现约亮 1 秒、灭 0.5 秒并循环的现象。

## 学习总结

本实验把“代码执行”与“真实硬件信号”连接了起来。`GPIO_ResetBits()`、`GPIO_SetBits()` 和 `delay_ms()` 不再只是程序语句，而是可以在 PC13 上测量到的电平变化和时间波形。逻辑分析仪能够帮助验证延时、时序和通信过程，是后续学习 I²C、OLED、传感器与电机控制的重要调试工具。

## 工程结构

```text
Day22_Logic_Analyzer_Basics/
├── Project/             # Keil 工程与工程配置
├── USER/                # main.c、中断与芯片系统文件
├── HARDWARE/            # LED 驱动
├── SYSTEM/              # delay、sys、usart
├── CORE/                # CMSIS Core 与 STM32 启动文件
├── STM32F10x_FWLib/     # STM32F10x 标准外设库
├── Images/              # 实验截图目录
├── .gitignore
└── README.md
```

## 打开工程

使用 Keil µVision 5 打开：

```text
Project/Day22_Logic_Analyzer_Basics.uvprojx
```

工程中的源码和头文件均使用相对路径引用。首次编译时，Keil 会在 `Project/Objects` 和 `Project/Listings` 下重新生成编译产物；这些目录已由 `.gitignore` 排除，不需要提交到 GitHub。

## 实验截图

原始学习工程未包含截图文件。实际采集逻辑分析仪波形后，可将图片放入 `Images/` 目录并在本节补充展示。
