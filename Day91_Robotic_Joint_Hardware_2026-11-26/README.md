# Day91｜完全体机器人关节硬件介绍

学习日期为 2026-11-26，对应 STM32 手册第五章第十一节《完全体机器人关节 - 硬件介绍》。本人依次读过主控制器、第一编码器和第二编码器三个小标题并完成讨论与口头自测。当日先完成主控制器，随后本人两次要求继续，实际读完本节。助手用飞书官方 CLI 以用户身份读取当日范围，核对版本 85263，并查看随讲配图。第十二节仅为预读和查证第二编码器调用，尚未进入本人正式学习。

## 实际阅读范围

| 组次 | 起点与停点 | 完成的读图内容 |
| --- | --- | --- |
| 1 主控制器电路 | 从[第十一节标题与三块板总图](https://my.feishu.cn/wiki/F1XQwrahqiF2ttknUyecU7qpnre#TTGcdwwB7obVghxJmIocrOain5R)读至[相电流输出的 RC 段](https://my.feishu.cn/wiki/F1XQwrahqiF2ttknUyecU7qpnre#SE13d7VVsogzOPxz1VQc8FZanjf)，在第一编码器标题前停 | 主图与子页、逐级降压、地网连接、PWM 驱动、母线电压和三相电流反馈 |
| 2 第一编码器电路 | 从[标题及圆板和原理图](https://my.feishu.cn/wiki/F1XQwrahqiF2ttknUyecU7qpnre#Acf5d0atJo6mPoxuyFucKe1enyc)读至开机和运行分工，停在第二编码器标题前 | AS5047P 的 SPI 单圈机械角、AB 增量计数、预留索引 I/Z、标定零位 |
| 3 第二编码器电路 | 从[标题及 C 形板](https://my.feishu.cn/wiki/F1XQwrahqiF2ttknUyecU7qpnre#VQ8cdV7qAoB0V1xTucNcft8XnUf)读至[末尾解释图后的说明](https://my.feishu.cn/wiki/F1XQwrahqiF2ttknUyecU7qpnre#TLZAddfw2oKO79xfytocI6JAn7d)，停在第十二节标题前 | 磁环、四个线性霍尔、ADS1115 经 I²C 传递读数、四路波形解算一圈内角度 |

## 关键原理和实际作答

### 主控制器

主图 `control.SchDoc` 连接电源、STM32F405 和驱动三个模块。电源从母线逐级得到 12 V、5 V、3.3 V，`AGND` 和 `PGND` 在给定原图中分别经 0 Ω 接到 `GND`。母线采样经 18 kΩ 与 1 kΩ 分压（约为 1/19），再由跟随器缓冲到 PA0。三路 PWM 由 PA8/9/10 转到半桥栅极驱动，三相反馈 ISA/ISB/ISC 回到 PA1/4/5。两组第一编码器接口与一路第二编码器接口从驱动页引出。

相线电流经 `SH` 和 `OUT` 之间的 1 mΩ 采样电阻。INA181A2 的 `IN+` 接 `SH`，`IN−` 接 `OUT`，差分增益为 50，零电流标称基准是 1.65 V。末尾 RC 的串联位装 0 Ω，原设计未按非零电阻配置该低通。算例中，`SH → OUT` 为正，+2 A 对应约 1.75 V。

**独立答对。** 当零电流基准为 1.65 V，ADC 读 1.55 V 时，本人答「out流向sh，因为比零电流电压小……负压差」。助手另补算放大器输出差 −0.10 V，采样电阻两端差 −2 mV，方向约定下对应 −2 A。电流大小由助手补算。完整供电与布线、实测偏置未独立验证。

### 第一编码器

本人读完后独立说出 AB 为增量式输出，SPI 读取绝对角。后续自测问「转子静止，开机清零 AB 计数器，能否直接视为电角度 0°」。本人独立答「不行，还得知道此时转子具体的机械角，是通过 SPI 读取的」。助手补足机械角到电角度需要事先建立的零位和极对数。装配关系保持时，SPI 可重新测当前单圈角，定时器接着以 AB 计数跟踪变化。完整的电角度换算和真实启停对齐属于已讲待验证。

### 第二编码器

本人独立说出四个霍尔相隔 90°，各路电压近似正弦、相位依次错开，组合可识别一圈内的位置。助手澄清磁环随输出轴转，霍尔固定在 C 形板，霍尔输出表示其敏感方向的磁场分量。1.65 V 是 3.3 V 供电时的标称中点，四路由 ADS1115 复用转换，通过 I²C 传回 STM32。教材末图的上半幅画成对称的两对 N/S 极，一圈定位的论证按图例中机械一圈对应一个信号周期的模型理解。实物磁环极数未核对。

**经范例引导答对。** 助手用 0° 与 180° 举例，让本人对照 30°、150° 时同样的正弦读数如何区分。本人答「30°高于1.65，150°低于1.65，我是根据你给我的例子推出来的」。本人追问算法是否在芯片内部。助手确认 ADS1115 负责采样，软件负责解角。完整手算、消除实测通道偏差和实板精度属于已讲待验证。

## 工程资料与验证状态

本节随附 `Hardware_Reference/` 三套**教材参考设计**的原项目、原理图与 PCB 文件。本人提供资料目录并读原理图。参考设计保留架构和作者原始文件，库设置仍引用作者路径，继续编辑元件可在原始资料目录中使用 `AD完整元件库` 重新关联。

| 设计 | 原理图入口 |
| --- | --- |
| STM32F405 主控制器 | [control.SchDoc](Hardware_Reference/slyfocv2/control.SchDoc) |
| AS5047P 第一编码器 | [原理图.SchDoc](Hardware_Reference/slyfocv1_encoder_board/原理图.SchDoc) |
| 四霍尔第二编码器 | [Sheet1.SchDoc](Hardware_Reference/slyfocv1_sec_encoder/Sheet1.SchDoc) |

- **静态核对。** 助手核对教材图、三个设计工程的文件组成与配套 C 源码。`ads1115.c` 第 42 行 `calculateMotorPosition()` 计算两组对置通道差值，调用 `atan2(y,x)` 并转换为度。第 65 行起 `ADS1115_Channel_Init()` 依次读 AIN0～AIN3，第 88 行写入 `chuzhoujiaodu`。`my_main.c` 第 66 行在 `StartTask02()` 调用它。第 185 行附近位置外环示例读取第一编码器的转子位置。源码相对于原资料根目录位于 `2.software/slyfoc_v2/User_SYS/`。
- **编译。** 本日无构建记录。
- **软件测试。** 本日无运行、仿真或自动化测试记录。
- **真实硬件。** 本日无焊接、上电、实测读角、采样或试转。资料整理与本次读图不构成硬件验收。第九节原有焊接暂缓状态沿用。

原资料为本人提供的「完全体FOC机器人关节硬软件资料」下的 `1.hardware` 和 `2.software`。学习整理及源文件索引单独记录。无本日工程代码修改。

## 下次准确起点

从[第十二节《完全体机器人关节 - 算法代码介绍》标题](https://my.feishu.cn/wiki/F1XQwrahqiF2ttknUyecU7qpnre#BIi4dUgcXoRlBXxT7YRcAN9Un0g)和[「外设及框架分析」](https://my.feishu.cn/wiki/F1XQwrahqiF2ttknUyecU7qpnre#KAEVdndHMoGbj2xZIk2cYUKEnUg)开始，读外设表和末尾关于定时器中断、FreeRTOS 任务的总览，停在「代码分析」前。先给准确范围和一个目标，等本人自读，再按图串讲。三项独立微技能沿用上述作答证据，不重复旧测试。
