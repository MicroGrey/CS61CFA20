# Lab 01
## 环境配置
```bash
sudo apt install build-essential cgdb valgrind
```
## cgdb
### 运行
#### 1. 编译调试程序
```bash
# 必须添加 -g 选项
gcc -g main.c -o demo
```

#### 2. 启动CGDB
```bash
cgdb demo
```

#### 3. 界面概览
```
[代码窗口]  ← 用方向键浏览
----------  ← 窗口分隔线
(gdb)       ← 命令输入区
```

---

### 核心操作指令

#### 通用快捷键
| 按键 | 功能              |
|------|-------------------|
| ESC  | 进入命令模式       |
| i    | 返回GDB输入模式    |
| o    | 切换窗口焦点       |
| /    | 源代码搜索         |

#### 调试控制
```gdb
b main     # 在main函数设断点
r          # 启动程序
n          # 单步跳过（不进入函数）
s          # 单步进入函数
c          # 继续运行
p var      # 打印变量值
q          # 退出
```

#### 窗口管理
| 快捷键 | 功能               |
|--------|--------------------|
| :resize | 调整窗口比例       |
| :split | 水平分屏显示新文件 |
| :tab   | 新建标签页         |

---

### 示例

#### 调试流程演示
1. 编写测试代码
```c
// debug_demo.c
#include <stdio.h>

int sum(int a, int b) {
    return a + b;
}

int main() {
    int result = sum(3, 4);
    printf("Result: %d\n", result);
    return 0;
}
```

2. 编译调试版本
```bash
gcc -g debug_demo.c -o debug_demo
```

3. 启动CGDB
```bash
cgdb debug_demo
```

4. 关键操作记录
```gdb
b main      # 设置断点
r           # 运行到main函数
s           # 进入sum函数
p a         # 查看参数a的值
bt          # 显示调用堆栈
```

---

### 常见问题解决

#### Q1: 断点不生效？
- 检查编译时是否添加 `-g` 参数
- 确认源代码未被修改后重新编译

#### Q2: 显示"找不到源代码"？
1. 在GDB命令区输入：
```gdb
dir /path/to/source
```

#### Q3: 界面显示错乱？
- 尝试重置终端尺寸
- 使用 `:resize` 调整窗口比例

---

### 进阶技巧

#### 1. 配置文件
```bash
# ~/.cgdb/cgdbrc
set ignorecase  # 搜索忽略大小写
set ts=4        # 设置缩进为4空格
```

#### 2. 可视化内存查看
```gdb
x/8xb &variable  # 以16进制查看内存
```

#### 3. 多进程调试
```gdb
set follow-fork-mode child  # 跟踪子进程
```

---

