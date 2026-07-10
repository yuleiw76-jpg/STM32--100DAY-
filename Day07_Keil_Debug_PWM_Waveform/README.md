今日目标

今天没有学习新的外设，而是学习了 Keil 的 Debug 调试功能，并通过 Logic Analyzer 观察 PWM 波形。

今天的核心目标是：

1. 学会使用 Keil Debug 检查程序执行流程
2. 学会观察变量值的变化
3. 学会通过 Logic Analyzer 查看 PWM 波形
4. 理解不同角度对应的 PWM 高电平宽度变化
5. 学会用调试工具判断问题，而不是只靠猜
今天学了
① Keil Debug 的基本作用

今天理解到，Debug 不是新的外设，而是用来检查程序运行过程的工具。

以前我写完程序后，主要是直接烧录，然后看实验现象是否正确。
今天开始学习通过 Debug 来观察：

1. 程序执行到哪里了
2. 某个 if 分支有没有进去
3. 变量当前是多少
4. 函数有没有被调用
5. PWM 波形是否真的发生变化

这让我意识到，调试不是简单地“看程序能不能跑”，而是要学会分析程序为什么这样运行。

② 学习了 Run to Cursor Line

今天学习了 Keil Debug 里的：

Run to Cursor Line

它的意思是：

程序从当前停止的位置开始运行，直到运行到光标所在的那一行再停下来。

我的理解是：

光标 = 我鼠标点到的代码位置
黄色箭头 = 程序当前执行到的位置

所以 Run to Cursor Line 就是：

让程序从黄色箭头当前位置开始运行
一直跑到我鼠标光标所在的代码行
然后暂停

这个功能可以用来判断程序有没有执行到某一行。

比如把光标放到：

Servo_SetAngle(angle);

如果程序能停到这一行，就说明程序确实运行到了这里。

③ 学会观察变量

今天在 Watch 窗口里观察了变量：

angle
ccr

其中：

angle：当前舵机目标角度
ccr：Servo_SetAngle() 函数内部计算出来的比较值

一开始我发现 ccr 显示：

<not in scope>

后来理解到，ccr 是 Servo_SetAngle() 函数里的局部变量。

它只在这个函数内部有效：

void Servo_SetAngle(u16 angle)
{
    u16 ccr;

    ccr = SERVO_MIN_CCR + (u32)angle * (SERVO_MAX_CCR - SERVO_MIN_CCR) / SERVO_MAX_ANGLE;

    TIM_SetCompare1(TIM1, ccr);
}

当程序不在 Servo_SetAngle() 函数内部时，Keil 就看不到 ccr，所以显示 <not in scope>。

这让我重新理解了 C 语言里的作用域：

局部变量只在它所在的函数内部有效。
函数执行完后，这个局部变量就不能在外部直接观察了。

如果想看最终的 CCR 值，可以直接观察：

TIM1->CCR1

因为 TIM1->CCR1 才是真正被修改的 PWM 比较寄存器。

④ 观察了 PWM 波形

今天通过 Keil 的 Logic Analyzer 看到了 PWM 波形。

以前我只是知道：

CCR = 500  → 高电平约 0.5ms
CCR = 1500 → 高电平约 1.5ms
CCR = 2500 → 高电平约 2.5ms

今天第一次在波形图里真正看到了 PWM 的样子。

这让我更直观地理解了：

PWM 不是一个抽象概念
而是真实存在的高低电平波形
⑤ 理解了 Grid 时间刻度的重要性

今天一开始我改变了舵机角度，但是发现 Logic Analyzer 里的 PWM 占空比好像没有明显变化。

后来发现不是代码错，而是：

Grid 时间刻度设置得太大

当 Grid = 5ms 时，而舵机 PWM 的高电平时间只在：

0.5ms ~ 2.5ms

之间变化，所以在图上看起来不明显。

把 Grid 改小之后，就可以清楚看到不同角度对应的高电平宽度变化：

0°    → 高电平约 0.5ms
45°   → 高电平约 1.0ms
90°   → 高电平约 1.5ms
135°  → 高电平约 2.0ms
180°  → 高电平约 2.5ms

这个发现很重要：

不是 PWM 没变，而是观察尺度不合适，导致变化看不明显。

以后调试 PWM、电机、编码器、传感器信号时，不能只看有没有波形，还要选择合适的时间刻度。

⑥ 进一步理解了 PSC、ARR、CCR

今天通过观察波形，进一步理解了：

PSC：决定定时器计数速度
ARR：决定 PWM 周期
CCR：决定 PWM 高电平时间

也就是：

PSC + ARR 决定 PWM 频率
CCR 决定 PWM 占空比

在本实验中：

PSC = 72 - 1
ARR = 20000 - 1

所以：

定时器计数频率 = 1MHz
1 个计数 = 1us
PWM 周期 = 20000us = 20ms
PWM 频率 = 50Hz

当角度变化时，Servo_SetAngle() 会计算新的 CCR：

ccr = SERVO_MIN_CCR + (u32)angle * (SERVO_MAX_CCR - SERVO_MIN_CCR) / SERVO_MAX_ANGLE;

然后通过：

TIM_SetCompare1(TIM1, ccr);

修改 TIM1_CCR1，从而改变 PWM 的高电平时间。

实验情况

今天主要进行了 Debug 和波形观察实验。

实验现象：

1. 能够进入 Keil Debug 模式
2. 能够使用 Run to Cursor Line 运行到指定代码行
3. 能够在 Watch 窗口观察 angle 变量
4. 理解了 ccr 显示 <not in scope> 的原因
5. 能够通过 Logic Analyzer 观察 PWM 波形
6. 修改 Grid 后，可以看到不同角度对应的 PWM 高电平宽度变化

通过观察发现：

0°、45°、90°、135°、180° 对应的 PWM 高电平时间确实不同

说明 Servo_SetAngle() 的角度到 CCR 转换逻辑是有效的。

最大收获

今天最大的收获是：

我第一次真正通过调试工具看到了 PWM 波形，并且知道可以通过 Debug 和 Logic Analyzer 验证程序，而不是只靠实验现象猜测。

以前我看到舵机动了，就觉得程序成功了。
今天我开始知道，还可以进一步验证：

1. angle 变量有没有变化
2. Servo_SetAngle() 有没有执行
3. CCR 是否真的改变
4. PA8 上的 PWM 波形是否正确
5. 高电平时间是否符合理论计算

这让我从“看现象”开始进入“查过程”。

最大 BUG
BUG005：Keil 工程路径错误

现象：

Keil 弹窗提示：

servo.h contains an incorrect path

并且工程树里的 servo.c 出现黄色感叹号。

原因：

HARDWARE 里的 SERVO 文件夹名字被修改，导致 Keil 工程中记录的路径和真实文件路径不一致。

Keil 原来记录的是：

HARDWARE\SERVO\servo.h

但真实文件夹名字发生变化后，Keil 就找不到对应文件。

解决：

把文件夹名字改回：

SERVO

之后工程可以正常编译。

总结：

文件在电脑里存在，不代表 Keil 工程路径一定正确。
Keil 工程树里出现黄色感叹号，通常说明文件路径断了。

以后让 Codex 整理文件时，需要明确要求：

只允许在 GitHub_Upload 文件夹中整理文件，禁止修改、重命名、移动原始 Keil 工程中的文件夹或文件。
今天最困惑的是

今天最困惑的是：

为什么我改变了 angle，理论上 CCR 应该变化，但一开始看到的 PWM 占空比好像没有明显变化？

后来理解到有几个原因：

1. angle 变化后，必须执行 Servo_SetAngle(angle)，CCR 才会真正改变。
2. ccr 是局部变量，程序不在 Servo_SetAngle() 内部时，Watch 里看不到。
3. Logic Analyzer 的 Grid 太大时，0.5ms~2.5ms 的高电平变化看起来不明显。
4. 调试时不仅要看代码，还要确认观察工具的设置是否合适。

最后把 Grid 改小后，PWM 高电平宽度变化就能看清楚了。

今天暴露出的薄弱点

今天暴露出的薄弱点主要有：

1. 对 Keil Debug 的按钮还不够熟悉。
2. 对局部变量作用域的理解还需要继续巩固。
3. 对 Watch 窗口、寄存器窗口、Logic Analyzer 的使用还不熟练。
4. 对 Keil 工程路径管理还不够敏感。
5. 对调试工具的时间尺度、观察尺度还没有形成习惯。

这说明以后不能只会写代码，还要学会用工具验证代码。

新的理解

今天最大的一个新理解是：

代码可能是对的，但如果调试工具的观察尺度不合适，也会误以为现象不对。

比如这次 PWM 波形其实已经随着 CCR 改变了，只是一开始 Grid 设置太大，看起来占空比没有变化。

这让我意识到，调试时要同时检查三件事：

1. 代码逻辑是否正确
2. 变量和寄存器是否正确变化
3. 观察工具的设置是否合适
明天准备学

明天建议学习：

OLED 显示

但在进入 OLED 之前，可以先简单复盘今天 Debug 的内容：

1. Run to Cursor Line 是什么
2. 断点有什么用
3. Watch 窗口怎么看变量
4. Logic Analyzer 怎么看 PWM 波形
5. 为什么 Grid 设置会影响观察效果

OLED 显示以后会很有用，因为智能车调试时可以把传感器值、角度、速度、误差等数据显示出来。

这对后面学习循迹小车和 PID 调试非常重要。