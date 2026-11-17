# 第八节来源与原始整合包索引｜2026-11-17

## 九张截图

全部路径已检查存在，按用户提供顺序阅读。截图重叠部分按连续内容处理。

| 顺序 | 材料 | 可见内容 |
| --- | --- | --- |
| 1 | [210244](<C:/Users/YueWang/Pictures/Screenshots/屏幕截图 2026-11-17 210244.png>) | 第八节标题、分立三相桥、栅极电阻和二极管、EG3014、自举电容、DRV8313引入 |
| 2 | [210251](<C:/Users/YueWang/Pictures/Screenshots/屏幕截图 2026-11-17 210251.png>) | DRV8313内部框图、电荷泵、控制输入、供电和状态引脚、封装 |
| 3 | [210300](<C:/Users/YueWang/Pictures/Screenshots/屏幕截图 2026-11-17 210300.png>) | 底部散热焊盘、DRV8313接线、整板电源和CAN、AS5600与增量编码器比较 |
| 4 | [210309](<C:/Users/YueWang/Pictures/Screenshots/屏幕截图 2026-11-17 210309.png>) | 电机与驱动板实物示例、轴端磁铁和安装关系 |
| 5 | [210321](<C:/Users/YueWang/Pictures/Screenshots/屏幕截图 2026-11-17 210321.png>) | 直径方向两极磁化与轴向磁化、轴端固定磁铁 |
| 6 | [210329](<C:/Users/YueWang/Pictures/Screenshots/屏幕截图 2026-11-17 210329.png>) | 磁铁定位件、胶水、固定居中及角度检测的联系 |
| 7 | [210336](<C:/Users/YueWang/Pictures/Screenshots/屏幕截图 2026-11-17 210336.png>) | 磁铁与芯片间距、圆形PCB正反面、固定孔和焊盘属性 |
| 8 | [210344](<C:/Users/YueWang/Pictures/Screenshots/屏幕截图 2026-11-17 210344.png>) | 固定孔属性、M3铜柱、孔径和Plated选项 |
| 9 | [210351](<C:/Users/YueWang/Pictures/Screenshots/屏幕截图 2026-11-17 210351.png>) | 机械孔最终参数和本节最后的PCB布局注意事项 |

## 用户提供的第五章开发整合包

原始位置：[FOC开发整合包](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/>)。本日只读清点为182个文件；数量包括工程附带文件，不代表182个文件均已逐一审阅。

- [原理图.SchDoc](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/小功率FOC驱动器电路/原理图.SchDoc>)、[PCB.PcbDoc](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/小功率FOC驱动器电路/PCB.PcbDoc>)：确认文件存在，未打开Altium工程进行完整电气或布局审查。
- [STM32工程](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/FOC控制STM32F103C8T6代码/USER/LED.uvprojx>)：用户提供的教材参考工程，本日未编译、修改或执行。
- [FOC器件选型BOM表.xlsx](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/FOC器件选型BOM表.xlsx>)：确认存在，未逐项核对BOM。
- [支架STL](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/2804电机支架.STL>)、[支架SLDPRT](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/2804电机支架.SLDPRT>)：确认存在，未修改、打印或装配。

## 本日主助手亲自静态阅读的源码

| 文件 | 核对内容 |
| --- | --- |
| [main.c](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/FOC控制STM32F103C8T6代码/USER/main.c:11>) | 初始化和循环；21—22行读角度、调用固定Uq/Ud的调制 |
| [FOC.c](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/FOC控制STM32F103C8T6代码/HARDWARE/FOC/FOC.c:13>) | Uq/Ud/angle参数、调制计算、写CCR |
| [encoder.c](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/FOC控制STM32F103C8T6代码/HARDWARE/ENCODER/encoder.c:182>) | I2C引脚宏、原始角读取、跨圈、机械弧度乘7 |
| [tim1.c](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/FOC控制STM32F103C8T6代码/HARDWARE/TIM1/tim1.c:33>) | 向下计数、PWM1和通道初始化片段 |
| [motor.c](<C:/Users/YueWang/Downloads/第五章FOC开发整合包/FOC开发整合包/FOC控制STM32F103C8T6代码/HARDWARE/MOTOR/motor.c:6>) | 上电施加比较值、延时、读取零位的动作 |

助手的只读核对不能记为用户独立写出、逐行读懂或上板验证。

## 对照资料

- [TI DRV8313原厂数据手册](https://www.ti.com/lit/ds/symlink/drv8313.pdf)：输入真值表、电荷泵、V3P3旁路和散热焊盘。
- [TI MOSFET栅极驱动原理](https://www.ti.com/lit/ml/slua618a/slua618a.pdf)：栅极充放电、VGS和高侧驱动。
- [Microchip Gate Drive Control](https://onlinedocs.microchip.com/oxy/GUID-75860F84-C078-4B0B-B234-2A7B365CFBF9-en-US-1/GUID-09919803-0FF8-4856-B6BD-7F03184BF613.html)：栅极电阻和关断二极管的作用。
- [ams AS5600原厂手册，SparkFun托管副本](https://cdn.sparkfun.com/assets/f/a/b/b/a/AS5600_Datasheet.pdf)：直径方向磁化、角度读取、典型气隙与AGC。原厂旧直链本次返回404，因此查阅的是原厂文档托管副本。
