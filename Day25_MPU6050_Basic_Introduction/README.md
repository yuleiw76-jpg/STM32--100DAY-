STM32 100Days Day25 总结
MPU6050 + DMP 初探（调试篇）
今日目标

完成 STM32 与 MPU6050 通信，并尝试使用 DMP 获取姿态角（Pitch、Roll、Yaw）。

今日完成内容
1. MPU6050 基础通信流程理解

今天深入了解了 MPU6050 的初始化流程：

IIC初始化
    ↓
MPU6050初始化
    ↓
DMP初始化
    ↓
读取FIFO数据
    ↓
计算姿态角

理解了：

MPU6050通过IIC通信
WHO_AM_I寄存器用于确认设备身份
DMP内部已经完成姿态解算，不需要自己处理陀螺仪积分
2. 成功运行 MPU6050 初始化流程

之前成功显示：

MPU OK

说明：

OLED正常工作
STM32程序正常运行
MPU6050初始化代码能够执行
3. 尝试DMP初始化

学习了：

DMP_Init();

内部主要步骤：

mpu_init()

↓

mpu_set_sensors()

↓

mpu_configure_fifo()

↓

dmp_load_motion_driver_firmware()

↓

dmp_enable_feature()

↓

mpu_set_dmp_state()

理解了DMP固件加载和FIFO数据流。

4. 学习Read_DMP函数

分析：

dmp_read_fifo()

获取：

quat[4]

四元数：

q0 q1 q2 q3

然后转换：

Pitch
Roll
Yaw

对应：

俯仰角
横滚角
偏航角
今日遇到的问题
1. 两套软件IIC问题

今天重点排查：

OLED与MPU6050的软件IIC关系。

最终确认：

当前工程设计：

OLED:
独立软件IIC

MPU6050:
独立软件IIC

不是简单共用一条线。

2. MPU6050 WHO_AM_I失败

出现：

WHO ERR

进一步定位：

问题可能来自：

MPU6050 IIC通信
PA4/PA5配置
ACK返回
地址读取
3. 修改过程中OLED黑屏

由于多次修改：

IIC引脚
main初始化顺序
DMP调用位置

导致OLED测试也受到影响。

最终决定：

停止继续堆修改。

今日最大收获

今天最大的收获不是让MPU6050动起来。

而是第一次真正经历了一次嵌入式调试：

以前：

代码跑不起来 = 不会

现在：

代码跑不起来 = 分析通信链路、硬件连接、初始化顺序、模块关系

这就是嵌入式工程师日常。

明日计划

重新复制工程：

Day25_MPU6050_backup

按照最小变量原则：

Step1

恢复OLED单独显示

↓

Step2

确认MPU6050 WHO_AM_I

↓

Step3

读取MPU6050原始数据

↓

Step4

开启DMP

↓

Step5

OLED显示姿态角

今日一句话总结

今天没有成功显示姿态角，但完成了从“调用代码”到“理解通信链路”的转变。嵌入式学习不是每天都成功运行，而是在一次次失败中学会定位问题。