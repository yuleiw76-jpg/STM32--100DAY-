# Day65｜独立看门狗与窗口看门狗

学习起止日期：2026-10-06 至 2026-10-06。对应手册第三章第四节“独立看门狗和窗口看门狗（已精进）”。本节按“独立看门狗完成对比实验、窗口看门狗理解概念并读懂示例”的目标收尾，材料仅在本地归档，供手动上传 GitHub。

| 内容 | 本次完成程度 |
| --- | --- |
| 独立看门狗 IWDG | 参数、喂狗与复位机制已理解；用户确认按键故障与恢复正确中断入口的对比实验成功 |
| 窗口看门狗 WWDG | 已完成刷新窗口、提前唤醒中断与示例代码阅读；未做实物实验 |
| AFIO 初始化顺序的旁支问题 | 现象已记录，原因待确认；尚未断电复测，用户同意保留不确定性后收尾 |

## 学习记录与实验截图

- [每日学习记录、参数计算、实验结果与待确认事项](README_2026-10-06.md)
- [独立看门狗的 ok 输出截图](Evidence/2026-10-06_IWDG_OK_Output.png)
- [恢复正确 EXTI4 入口后的 4 与 ok 输出截图](Evidence/2026-10-06_IWDG_Correct_EXTI4_Output.png)

## 工程快照

- 独立看门狗：[主程序](Code/IWDG/USER/main.c)、[初始化与喂狗](Code/IWDG/HARDWARE/IWDG/iwdg.c)、[外部中断](Code/IWDG/HARDWARE/EXTI/exti.c)、[Keil 工程](Code/IWDG/USER/LED.uvprojx)。
- 窗口看门狗：[主程序](Code/WWDG/USER/main.c)、[初始化与提前唤醒中断](Code/WWDG/HARDWARE/WWDG/wwdg.c)、[外部中断示例](Code/WWDG/HARDWARE/EXTI/exti.c)、[Keil 工程](Code/WWDG/USER/LED.uvprojx)。

两个 `Code` 子目录是归档时读取到的完整工程快照，保留原字节、原编码及原有构建产物。本次整理没有修改、重新编译或烧录源工程；已有构建产物不作为本次新编译、实物测试或源码一致性的证明。

重要边界：IWDG 快照中已存在正确的 `EXTI4_IRQHandler`；WWDG 示例仍有“配置 EXTI4_IRQn、却定义 EXTI0_IRQHandler”的名称不匹配，原样保留并在学习记录中注明，不应把它当作已修复、已上板验证的版本。

GitHub 按完成的手册小节计数，本节资料为 Day65；抖音口播承接前稿，独立记为第67天，保存在 GitHub 目录之外。未执行 Git 提交、上传或推送。
