# 2026-10-14｜HAL闪灯构建与证据摘要

本文件由助手只读核对用户现有构建日志后整理，不是助手重新构建的输出。原始日志含本机许可信息，未复制到待上传目录。

## 构建结果

| 项目 | 现有日志记录 |
| --- | --- |
| 工程/目标 | led |
| IDE | µVision V5.29.0.0 |
| 编译器 | Arm Compiler V5.06 update 6，build 750 |
| 启动文件 | assembling startup_stm32f103xb.s |
| 链接 | 完成并输出程序大小 |
| Code | 2776字节 |
| RO-data | 304字节 |
| RW-data | 16字节 |
| ZI-data | 1632字节 |
| 最终状态 | 0 Error(s), 0 Warning(s) |

当前工程已引用MDK-ARM/startup_stm32f103xb.s。归档中的255个保留源码、库文件和配置与工作目录逐文件SHA-256一致，17个Keil源码引用均存在。

## 实物与证据边界

用户确认修改下载接口后，灯可以正常亮灭。照片只记录点亮瞬间；没有独立周期测量、长时间统计或芯片中固件哈希验证。

| 原样副本 | SHA-256 |
| --- | --- |
| 2026-10-14_CubeMX_Missing_F1_Package.png | ED7B2EF9E101701C2BB00FB90A1649C902E3B3772BD551A72F1EB8C170FA5D6F |
| 2026-10-14_Keil_Startup_Link_Error.png | 98741650E7FA3ACFDD49920F82E92B45F174918DF10D63E20891AAFEE70F1680 |
| 2026-10-14_JLink_JTAG_Connection_Error.png | BA0C1A7FFD0C1D0826A9A3CC854A179059FBD3317B664DAE62E3DB56ACF1C77E |
| 2026-10-14_HAL_LED_Hardware.jpg | 45EF40917E432835D64C2070E50143B5E6C4039257CBA7CA2D031811B2C91372 |

4张图片副本均与用户原文件一致。没有复制账号登录页面、密码、验证码或含许可信息的原始构建日志。
