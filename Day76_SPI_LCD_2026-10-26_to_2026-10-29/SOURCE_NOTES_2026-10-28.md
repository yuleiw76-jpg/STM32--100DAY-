# 2026-10-28｜SPI1源码快照说明

原工程：`C:/Users/YueWang/Desktop/hal1/SPI1`。
快照入口：[SPI1.uvprojx](Code_2026-10-28/SPI1/MDK-ARM/SPI1.uvprojx)。

## 内容与完整性

归档共**189份文件，17,298,277字节**；逐项SHA256与原件一致，见[SOURCE_MANIFEST_2026-10-28.json](SOURCE_MANIFEST_2026-10-28.json)。
清单中的Path相对于 `Code_2026-10-28/SPI1`。

保留Core、HAL驱动、所需CMSIS头文件、启动文件、RTE配置、Keil工程及CubeMX .ioc。工程20个FilePath、5个IncludePath均在副本内存在。ST/CMSIS许可文本随源码保留。

未纳入本机调试选项、JLink日志/设置、原始build_log以及对象、AXF、HEX等构建输出。Keil本机工具与外部器件Pack不在快照内；使用者需有对应开发环境，固件包配置为STM32Cube FW_F1 V1.8.7。

## 主要文件

- [main.c](Code_2026-10-28/SPI1/Core/Src/main.c)：初始化与普通RGB循环填屏。
- [spi.c](Code_2026-10-28/SPI1/Core/Src/spi.c)：SPI1与PA4/PA5/PA7复用配置。
- [my.lcd.c](Code_2026-10-28/SPI1/Core/Src/my.lcd.c)：命令/数据发送、初始化、窗口与填屏。
- [my.lcd.h](Code_2026-10-28/SPI1/Core/Src/my.lcd.h)：PB10复位、PB11命令/数据控制、横屏尺寸和颜色宏。
- [SPI1.ioc](Code_2026-10-28/SPI1/SPI1.ioc)：本人配置的CubeMX工程。

保留原有文件名、源码内容及编码，没有因整理而纠正注释或改写实现。

## 版本范围

这是本日基础显示版本，不含分块加速函数、字符显示调用或DMA。HAL发送返回值等后续改进点没有在收尾中擅自修改。

当前证据支持基础RGB显示成功，不能据此声明触摸、所有显示功能或长期稳定性已通过。已有编译日志只做脱敏摘录，收尾没有重新编译或烧录。

本人在自行配置工程上完成移植与验证，侧聊协助细节未留存；不把归档说成助手本次代写或完全独立设计的驱动。
