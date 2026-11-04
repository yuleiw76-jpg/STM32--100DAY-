# 2026-11-04 原工程实测与样例构建

- 单块采样阶段。Watch 显示 300 点缓冲区、完成标志为 1，最小值 0，最大值 4095。[极值截图](adc-min-max.png)
- 重启逻辑接入后。两次 Watch 读数分别为 0xC5（197 块）和 0x2D0（720 块），相差 523 块。[第一次计数](blocks-197.png) · [第二次计数](blocks-720.png)
- 原工程当日保存的主程序与构建输出。构建日志为 0 错误、0 警告并生成 HEX。[原主程序](tested-main.c) · [原构建摘录](tested-build-output.txt)
- 独立样例在 Keil 中全量重建，0 错误、0 警告并生成 HEX。[样例构建摘录](example-rebuild-output.txt)
