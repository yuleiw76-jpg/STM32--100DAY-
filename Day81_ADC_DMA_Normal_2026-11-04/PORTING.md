# 将 Normal 模式分块采集移植到自己的工程

先在目标工程里建立 ADC 到缓冲区的路径，再复制应用代码。以下以同款 STM32F103C8T6 为例。

| 位置 | 检查或移植内容 |
| --- | --- |
| CubeMX 时钟与 ADC | 按目标工程的 PCLK2 确认 ADC 分频，本例 PCLK2 72 MHz ÷ 6 得到 12 MHz。选 ADC1_IN3，设置单通道、软件启动、连续转换及采样时间 |
| CubeMX DMA 和 NVIC | 给 ADC1 加 DMA1 Channel 1，选择 Normal、外设到内存、外设固定、内存自增、半字对半字，开启该通道的 DMA 中断 |
| 生成的初始化 | 在 `main()` 中核对 `MX_DMA_Init()` 先于 `MX_ADC1_Init()`。在 `adc.c` 中找到 `__HAL_LINKDMA`，在中断文件中找到 `DMA1_Channel1_IRQHandler()` 调用 `HAL_DMA_IRQHandler()` |
| `USER CODE PD / PV` | 样本数、`uint16_t` 缓冲区、完成标志和 Watch 观察变量 |
| `USER CODE 2` | 在外设初始化后启动信号源，校准 ADC，启动第一块采集。本例用 TIM2_CH3 提供信号，已有合适信号源的工程可省去 PWM 相关调用 |
| `USER CODE 4` | 将 ADC1 的完成处理并入 `HAL_ADC_ConvCpltCallback()`。有同名回调时合并分支 |
| `USER CODE 3` | 在主循环中处理整块，停止采集、清完成标志、再启动下一块 |

样本数同时用于数组长度、遍历边界和两次启动调用。`HAL_ADC_Start_DMA()` 的缓冲区形参是 `uint32_t *`，因此调用处做指针转换。实际搬运宽度取决于 DMA 两端的半字配置，`Length=300` 对应 300 个样本。

DMA 在 Normal 模式搬满后停止写这一块，ADC 的连续转换可继续到 `HAL_ADC_Stop_DMA()`。回调只负责通知，扫描极值留在主循环。先清旧标志再启动，能避免主循环把下一块的完成通知覆盖掉。重新启动之间存在采样间隙。换用 Circular 模式时，半满、全满通知指向对应半区，需要在 DMA 回卷覆盖前读完。

移植后可按顺序观察三件事：

1. PWM 回环建立后，`adc1_caiji` 可看到高、低两类读数，`adc_min`、`adc_max` 接近两端。
2. 运行后暂停观察 `adc_blocks_processed`，继续运行后再暂停，确认计数增加。
3. 若计数不动，依次看 PWM 是否已输出、DMA 是否在搬运、IRQ 是否分发到 HAL、完成回调名字是否为 `HAL_ADC_ConvCpltCallback`。今日的 `Conc` 拼写会让自写函数无法接到 HAL 的完成通知。第一块只有 0～3 的现象则在调整 PWM 启动顺序后消失。

源码和参数可从样例工程直接查阅。[采集 main.c](Code/ADC_DMA_Normal/Core/Src/main.c) · [ADC/DMA 绑定](Code/ADC_DMA_Normal/Core/Src/adc.c) · [DMA 中断入口](Code/ADC_DMA_Normal/Core/Src/stm32f1xx_it.c)
