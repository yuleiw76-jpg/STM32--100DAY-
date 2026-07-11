# Day08 - OLED 显示与月薪猫动画移植

## 今日学习目标

- 跑通 OLED 文字显示
- 理解 SCL 和 SDA 的作用
- 理解 OLED 字模和图片数组
- 学会使用 GitHub 搜索开源项目
- 将月薪猫动画帧移植到自己的 STM32 标准库工程
- 调整动画播放速度

## 硬件接线

- OLED GND → STM32 GND
- OLED VCC → STM32 3.3V
- OLED SCL → PA6
- OLED SDA → PA7
- KEY1 → PB4
- KEY2 → PB5

说明：

PA6、PA7 使用 GPIO 模拟 I2C。

## OLED 基础知识

- SCL 是时钟线，提供通信节拍
- SDA 是数据线，传输命令和显示数据
- OLED_Init() 用于初始化 GPIO 和 OLED 控制器
- OLED_Clear() 用于清除 OLED 显存内容
- 单色图片可以转换成由 0 和 1 组成的像素数组
- 128×64 单色图片每帧需要 1024 字节
- 多帧图片快速刷新形成动画

## 工程说明

### Course_Project

课程原始 OLED 工程，主要完成：

- OLED 初始化
- 字符显示
- 字符串显示
- 数字显示
- 按键切换显示内容

### YueXinCat_Project

在课程工程基础上完成：

- 引入 yuexinmiao_oled_frames.h
- 增加 OLED_ShowFrame()
- 显示一整帧 128×64 图片
- 循环播放 7 帧月薪猫动画
- 调整 YUEXINMIAO_FRAME_DELAY_MS 改变动画速度

## 关键代码

OLED_ShowFrame() 用于显示一整帧 128×64 的单色图片：

- OLED 分为 8 页
- 每页发送 128 字节
- 总共发送 1024 字节
- 每次显示一张完整的 128×64 图片

## BUG 记录

### BUG006：OLED 不显示月薪猫

现象：

- 编译成功
- 烧录成功
- OLED 没有显示图片

原因：

在 OLED_Init() 之前调用了 OLED_ShowFrame()。

错误顺序：

```c
OLED_ShowFrame();
OLED_Init();
OLED_Clear();
```

正确顺序：

```c
OLED_Init();
OLED_Clear();
OLED_ShowFrame();
```

### BUG007：动画帧率较低

现象：

月薪猫可以播放，但动画较慢。

原因：

- 默认每帧延时为 140ms
- Write_IIC_Data() 每发送一个字节都会重新执行 Start、地址发送和 Stop
- 一帧有 1024 字节，通信开销较大

当前解决：

将 YUEXINMIAO_FRAME_DELAY_MS 从 140 调整为更小数值。

后续优化：

连续发送一页或一整帧数据，减少重复的 Start 和 Stop。

## 今日收获

今天完成了一次完整的小型代码移植：

GitHub 搜索开源项目
→ 判断硬件和驱动兼容性
→ 提取动画帧数组
→ 分析自己的 OLED 驱动
→ 增加整帧显示函数
→ 排查初始化顺序问题
→ 成功播放动画

## 开源来源说明

在当前月薪猫帧文件中看到的来源注释为：由 assets/yuexinmiao_original.gif 生成，当前工作区来源为预处理后的 preview_128x64.gif。

未在当前工程或帧文件注释中找到明确的原始 GitHub 仓库名称、作者和 License。

本项目中的月薪猫动画帧来源于公开 GitHub 仓库，仅用于个人学习与技术研究。原始版权归原作者所有。由于未确认明确开源许可证，本仓库不声明对该素材拥有版权。
