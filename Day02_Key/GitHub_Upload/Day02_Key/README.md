# Day02 - STM32 按键输入实验

## 今日目标

学习 STM32 按键输入实验，通过两个按键控制板载 LED 的亮灭。

## 实验现象

- 按下按键 1，板载 LED 点亮。
- 按下按键 2，板载 LED 熄灭。

## 今日学习内容

### 1. GPIO 上拉输入模式

今天学习了 `GPIO_Mode_IPU` 上拉输入模式。

当 IO 口配置为上拉输入时：

- 按键没有按下时，IO 口默认读到高电平 `1`。
- 按键按下并接到 GND 时，IO 口读到低电平 `0`。

所以在本实验中，判断按键按下通常写成：

```c
if(KEY1 == 0)
{
    // KEY1 被按下
}
2. JTAG 和 SWD 复用问题

学习到 PB3、PB4、PA15 等 IO 口可能被 JTAG 调试功能占用。

如果要把这些引脚当作普通 GPIO 使用，需要关闭 JTAG，保留 SWD：

GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

我的理解是：

JTAG 和 SWD 都是单片机调试下载接口。
关闭 JTAG 可以释放部分 IO 口。
保留 SWD 后，仍然可以下载和调试程序。
3. static 关键字

今天学习了 static 修饰局部变量的作用。

static 修饰的局部变量只会初始化一次，函数结束后不会被销毁，下次进入函数时还能保留上一次的值。

例如：

static u8 key_up = 1;

如果没有 static，每次调用 KEY_Scan() 时，key_up 都会重新变成 1，无法记录上一次按键状态。

4. key_up 状态标志位

key_up 不是直接判断按键有没有被按下。

真正判断按键有没有按下的是：

KEY1 == 0
KEY2 == 0

key_up 的作用是判断：

这一次按下动作是否已经被处理过，是否允许再次触发。

我的理解：

key_up = 1：表示按键已经松开，允许检测下一次新的按下动作。
key_up = 0：表示这次按下已经被处理过，如果不支持连按，就不能重复触发。

这让我第一次接触到了“状态机”的思想。

5. 支持连按的逻辑

支持连按不是简单地把 KEY_Scan(0) 改成 KEY_Scan(1)。

更准确地说：

支持连按是指按键按下后先触发一次；如果一直按住不放，程序会每隔一段预设时间再次触发一次对应操作。

例如老式数码相机查看照片：

按一下：切换到下一张照片。
一直按住：照片持续往后翻。

所以支持连按的核心是：

按住不放时，程序周期性地产生新的按键事件。

6. 修改 IO 口的方法

今天尝试把原来的 PB4、PB5 改成 PA6、PA5。

修改 IO 口时，不能只改 GPIO_Pin，还要同时修改：

GPIO_Pin_x
GPIOA / GPIOB / GPIOC
RCC_APB2Periph_GPIOx
key.h 里的宏定义
实际硬件接线

例如这次我把按键改到 PA6、PA5 后，需要把 GPIOB 时钟改成 GPIOA 时钟：

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
今日 BUG
BUG001：Keil 工程路径错误

现象：

打开工程时提示：

key.c contains an incorrect path

后面又出现：

sys.h file not found

原因：

一开始只复制了部分工程文件夹，导致 Keil 工程中记录的相对路径找不到对应文件。

STM32 工程不只有 USER 文件夹，还依赖：

HARDWARE
SYSTEM
CORE
STM32F10x_FWLib

解决方法：

保持老师原始工程的完整目录结构，把这些文件夹放在正确的相对层级下。

BUG002：修改 IO 口后按键没反应

现象：

把 PB4、PB5 改成 PA6、PA5 后，按键没有反应。

原因：

修改了：

GPIO_Init(GPIOA, &GPIO_InitStructure);

但时钟仍然写的是：

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

也就是初始化的是 GPIOA，但打开的是 GPIOB 的时钟。

解决方法：

改成：

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
最大收获

今天最大的收获是第一次理解了“状态机”的思想。

以前我觉得变量只是用来存数据的，但今天发现变量还可以用来记录程序当前状态。

例如 key_up 这个变量，它在告诉程序：

当前按键是否已经被处理过？
是否允许下一次触发？
是否正在等待松手？
是否可以进入连按判断？

这让我意识到，嵌入式开发不是简单地让代码从上往下跑，而是要根据硬件状态和程序状态来设计逻辑。

今日困惑

状态机是否还有更多用法？

目前我的理解是：

状态机就是用变量记录程序当前状态，再根据状态决定下一步执行什么。

我猜后面在 OLED 菜单、短按长按双击、电机控制、小车运行模式、串口通信协议解析中，应该都会用到状态机。

明日计划
学习如何自己添加 .c 和 .h 文件
学习定时器中断