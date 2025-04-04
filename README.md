# 异步日志系统

## 250404
- 初步实现其功能，代码比较简单。

## 250405
- 日志添加类别信息
- 日志添加时间信息

日志输出结果为
```bash
[INFO] [2025-04-05 00:26:31] Starting application.
[INFO] [2025-04-05 00:26:31] User 42 performed login in 3.5 seconds.
[DEBUG] [2025-04-05 00:26:31] Hello World
[WARN] [2025-04-05 00:26:31] This is a message without placeholders.
[ERROR] [2025-04-05 00:26:31] Multiple placeholders: 1, 2, 3.
```