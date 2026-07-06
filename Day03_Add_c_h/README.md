Day03 - 自己添加 .c / .h 文件并复刻按键实验
今日目标

今天的目标是：
在 Day1 跑马灯工程的基础上，自己手动添加 key.c 和 key.h 文件，并复刻 Day2 的按键控制 LED 实验。

今天学了
① .c 文件使用 .h 文件中的内容时，需要 #include

如果一个 .c 文件要使用某个 .h 文件里声明的函数、宏定义或类型，就需要在开头写：

#include "xxx.h"

例如 main.c 中要使用按键函数，就需要写：

#include "key.h"

这样 main.c 才知道有 KEY_Init() 和 KEY_Scan() 这些函数可以调用。

② 同一个函数不能在多个 .c 文件中重复定义

今天遇到的核心问题是：

Symbol KEY_Scan multiply defined (by key.o and main.o)

意思是：

KEY_Scan 这个函数被重复定义了。

原因是我把 KEY_Scan() 的完整实现代码同时写到了 key.c 和 main.c 里面。

正确做法是：

key.h：只写函数声明
key.c：写函数具体实现
main.c：只调用函数

例如：

// key.h
u8 KEY_Scan(u8 mode);
// key.c
u8 KEY_Scan(u8 mode)
{
    // 具体实现代码
}
// main.c
a = KEY_Scan(0);
③ Keil 中新建 .c / .h 文件后，还需要添加进工程

在文件夹中新建 key.c 和 key.h 之后，Keil 不会自动知道它们的存在。

需要在 Keil 工程分组中手动添加文件，尤其是 .c 文件。

我的理解是：

文件夹里有这个文件，不代表 Keil 工程已经使用了这个文件。
必须把 .c 文件添加进 Keil 工程，编译器才会编译它。

.h 文件主要用于包含和声明，可以添加进工程方便查看，但真正必须参与编译的是 .c 文件。

④ 只有 0 Error(s) 后才会生成 LED.axf

今天我学到：

Keil 只有在编译、链接全部成功后，才会生成 .axf 文件。

如果还有错误，就会出现：

Target not created

这时候下载程序时就会报错：

Could not load file '..\OBJ\LED.axf'

所以正确流程应该是：

Rebuild
↓
确认 0 Error(s)
↓
生成 LED.axf
↓
Download 到单片机
⑤ .h 文件开头通常有防止重复包含的固定格式

例如 key.h 中：

#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif

我的理解是：

#ifndef / #define / #endif 可以防止头文件被重复包含，避免重复定义或编译错误。

这是以后自己写 .h 文件时需要养成的基本习惯。

实验情况

今天成功完成了 Day3 阶段性任务：

我从原来的 LED 跑马灯工程出发，手动添加了 key.c 和 key.h 文件，并配置了 Keil 工程文件分组。

在过程中，我遇到了 KEY_Scan 函数重复定义的链接错误。解决后重新编译，成功生成 LED.axf，并下载到单片机中。

最终实验现象成功复刻 Day2：

按下按键 1，板载 LED 点亮。
按下按键 2，板载 LED 熄灭。

这次不是直接运行老师给的完整工程，而是自己从旧工程一步步添加文件、配置工程、解决错误，最后实现相同功能。

最大收获

今天最大的收获是理解了 STM32 工程的模块化结构。

以前我只是跟着老师打开工程、运行代码。今天开始明白：

.h 文件负责声明
.c 文件负责实现
main.c 负责调用
Keil 工程负责组织和编译这些文件

我还理解了为什么同一个函数不能在多个 .c 文件中重复定义。

这让我对 .c / .h 文件的分工、Keil 工程结构、编译和链接过程有了更清晰的认识。

最大 BUG
BUG003：KEY_Scan 重复定义

现象：

编译时报错：

Symbol KEY_Scan multiply defined (by key.o and main.o)

原因：

我把 KEY_Scan() 函数的实现代码同时写在了 key.c 和 main.c 里。

这样编译时，key.c 会生成一个 KEY_Scan，main.c 也会生成一个 KEY_Scan。链接时就会发现同名函数重复定义。

解决方法：

把 KEY_Scan() 的实现代码只保留在 key.c 中。

结构改成：

key.h：函数声明
key.c：函数实现
main.c：函数调用

修改后重新编译，成功变成：

0 Error(s)

并成功下载到单片机。

今天最困惑的是

今天主要困惑的是：

为什么 .c 文件和 .h 文件要分开写？
为什么函数声明和函数实现不能随便放？

通过今天的实验，我初步理解到：

.h 文件像是对外说明书，告诉其他文件“我这里有哪些函数可以用”。

.c 文件才是真正实现功能的地方。

main.c 只需要包含 .h 文件，然后调用函数，不应该把所有函数实现都堆在 main.c 里面。

明天准备学

明天准备学习：

定时器中断

重点关注：

1. 什么是中断
2. 定时器为什么可以定时
3. 中断服务函数是什么
4. 定时器中断和 delay 延时有什么区别
5. 如何用定时器实现周期性任务