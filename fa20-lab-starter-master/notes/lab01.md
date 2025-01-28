# Lab 01
## 环境配置
```bash
sudo apt install gcc build-essential cgdb valgrind
```

## gcc
### 常用编译选项
| 选项          | 说明                          | 示例                    |
|---------------|-------------------------------|-------------------------|
| `-o`          | 指定输出文件名                | `gcc hello.c -o myapp` |
| `-Wall`       | 启用所有警告                  | `gcc -Wall test.c`     |
| `-g`          | 生成调试信息                  | `gcc -g debug.c`       |
| `-O`          | 优化级别（0-3）               | `gcc -O2 optimize.c`   |
| `-I`          | 添加头文件搜索路径            | `gcc -I./include src.c`|
| `-c`          | 只编译不链接                  | `gcc -c module.c`      |
| `-E`          | 仅执行预处理                  | `gcc -E src.c -o out.i` |
| `-S`          | 预处理+编译，生成汇编代码      | `gcc -S src.c -o out.s` |
| `-v`          | 显示详细编译过程              | `gcc -v hello.c`        |
| `-D<MACRO>[=VAL]`    | 定义预处理宏           | `-DDEBUG -DVERSION=1.2` |
| `-U<MACRO>`          | 取消宏定义             | `-UDEBUG`               |
| `-I<dir>`      | 添加头文件搜索路径            | `-I./include`          |
| `-M`          | 生成依赖规则（Makefile用）    | `gcc -M src.c`          |
| `-MM`          | 排除系统头文件的依赖规则      | `gcc -MM src.c`         |
| `-l<library>`        | 链接指定库           | `-lpthread -lm`         |
| `-L<dir>`            | 添加库文件搜索路径       | `-L/usr/local/lib`      |
| `-static`            | 强制静态链接          | `gcc -static prog.c`    |
| `-shared`    | 生成共享库           | `gcc -shared -fPIC -o libfoo.so foo.c` |
| `-Wl,<option>`       | 传递选项给链接器       | `-Wl,-rpath=/opt/lib`   |

#### 调试信息
| 选项                 | 说明                          |
|----------------------|-------------------------------|
| `-g`                 | 生成基础调试信息              |
| `-g3`                | 生成最大调试信息（含宏定义）   |
| `-ggdb`              | 生成GDB专用调试信息           |
| `-fno-eliminate-unused-debug-types` | 保留未使用的类型信息 |

#### 诊断控制
| 选项                 | 说明                          |
|----------------------|-------------------------------|
| `-Wall`              | 启用所有常见警告              |
| `-Wextra`            | 启用额外警告                  |
| `-Werror`            | 将警告视为错误                |
| `-Wno-<warning>`     | 禁用特定警告（如`-Wno-unused`）|
| `-fdiagnostics-color=always` | 彩色错误输出   |

#### 优化级别
| 选项                 | 说明                          |
|----------------------|-------------------------------|
| `-O0`                | 不优化（默认）                |
| `-O1`                | 基础优化                      |
| `-O2`                | 推荐优化级别                  |
| `-O3`                | 激进优化（可能增加代码体积）   |
| `-Os`                | 优化代码大小                  |
| `-Ofast`             | 启用所有优化，包括不符合标准的 |

#### 特定优化
| 选项                 | 说明                          |
|----------------------|-------------------------------|
| `-march=native`      | 针对本机CPU优化               |
| `-flto`              | 链接时优化                    |
| `-fomit-frame-pointer` | 省略栈帧指针（提高性能）     |
| `-funroll-loops`     | 循环展开优化                  |

#### 代码生成选项
| 选项                 | 说明                          | 应用场景                |
|----------------------|-------------------------------|-------------------------|
| `-std=<standard>`    | 指定C语言标准                 | `-std=c11 -std=gnu17`   |
| `-fPIC`              | 生成位置无关代码              | 共享库编译              |
| `-pthread`           | POSIX线程支持                 | 多线程程序              |
| `-ffunction-sections` | 将函数放入独立段             | 代码大小优化            |
| `-fdata-sections`    | 将数据放入独立段              | 配合`--gc-sections`使用 |

#### 安全加固选项
| 选项                 | 说明                          |
|----------------------|-------------------------------|
| `-fstack-protector`  | 启用栈保护                    |
| `-D_FORTIFY_SOURCE=2` | 运行时缓冲区溢出检测         |
| `-Wformat-security`  | 格式化字符串安全检查          |
| `-z relro`           | 设置重定位只读（需链接器支持）|

### 分步编译
```bash
# 1. 编译单个模块
gcc -c main.c       # 生成main.o
gcc -c utils.c      # 生成utils.o

# 2. 链接所有对象文件
gcc main.o utils.o -o app
```
### 一键编译
```bash
gcc main.c utils.c -o app
```

### 静态库与动态库

#### 创建静态库
```bash
# 1. 生成对象文件
gcc -c libmath.c

# 2. 打包为静态库
ar rcs libmath.a libmath.o
```

#### 使用静态库
```bash
gcc main.c -L. -lmath -o calc
```

#### 创建动态库
```bash
gcc -shared -fPIC libmath.c -o libmath.so
```

---

### 预处理与分步编译

#### 1. 预处理（生成.i文件）
```bash
gcc -E hello.c -o hello.i
```

#### 2. 编译（生成.s汇编）
```bash
gcc -S hello.i -o hello.s
```

#### 3. 汇编（生成.o目标文件）
```bash
gcc -c hello.s -o hello.o
```

#### 4. 链接（生成可执行文件）
```bash
gcc hello.o -o hello
```

---

## cgdb
### gdb介绍

#### 第一个调试程序

##### 1. 编写测试代码
```c
// hello.c
#include <stdio.h>

int main() {
    int count = 5;
    for(int i=0; i<count; i++){
        printf("Hello %d\n", i);
    }
    return 0;
}
```

##### 2. 编译调试版本
```bash
# 必须添加 -g 参数
gcc -g hello.c -o hello
```

##### 3. 启动GDB
```bash
gdb ./hello
```

---

#### 基础调试命令

##### 启动与退出
| 命令          | 说明              |
|---------------|-------------------|
| `gdb <程序名>` | 启动调试          |
| `quit` 或 `q`  | 退出GDB           |

##### 执行控制
| 命令 | 说明                   |
|------|------------------------|
| `run` 或 `r` | 开始运行程序           |
| `next` 或 `n` | 执行下一行（不进入函数）|
| `step` 或 `s` | 进入函数内部           |
| `continue` 或 `c` | 继续运行到下一个断点 |

##### 断点管理
```gdb
break main    # 在main函数设置断点
break 10      # 在第10行设置断点
info break    # 查看所有断点
delete 2      # 删除2号断点
```

##### 数据查看
| 命令                  | 说明                |
|-----------------------|---------------------|
| `print 变量名` 或 `p` | 打印变量值          |
| `backtrace` 或 `bt`   | 查看函数调用堆栈    |
| `info locals`         | 显示当前局部变量    |

---

#### 实战调试演示

##### 调试流程
1. 启动调试
```bash
gdb ./hello
```

2. 设置断点
```gdb
(gdb) break main
(gdb) break 6     # 在printf行设置断点
```

3. 运行程序
```gdb
(gdb) run
```

4. 逐步调试
```gdb
(gdb) next     # 执行int count=5
(gdb) print count  # 显示5
(gdb) continue # 运行到循环处
```

---

#### 常见问题解决

##### Q1: 程序崩溃如何定位？
1. 使用GDB运行崩溃程序
```bash
gdb ./崩溃程序
```
2. 输入以下命令定位问题
```gdb
(gdb) run
(gdb) backtrace  # 查看崩溃时的调用栈

```

##### Q2: 如何查看数组内容？
```gdb
(gdb) p *array@10  # 查看数组前10个元素
```

##### Q3: 修改变量值测试
```gdb
(gdb) set var count=3  # 修改循环次数
(gdb) continue         # 继续运行观察变化
```

---

#### 实用技巧

##### 1. 显示源代码
```gdb
list     # 显示当前代码
list 5,10  # 显示5-10行
```

##### 2. 监视点（数据断点）
```gdb
watch count  # 当count变化时暂停
```

##### 3. 多线程调试
```gdb
info threads    # 查看线程
thread 2        # 切换到2号线程
```

---

### 运行
#### 1. 编译调试程序
```bash
# 必须添加 -g 选项
gcc -g main.c -o demo
```

#### 2. 启动CGDB
```bash
cgdb demo
# 带参数启动
cgdb --args ./input_demo arg1 arg2
# 带参数启动，文件重定向（优先使用）
cgdb --args ./program < input.dat
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

#### Q4: 传参方式
```gdb
#方式一
(gdb) set args arg1 "带空格参数"
(gdb) show args
(gdb) run

# 方式二，使用预输入（避免阻塞）
(gdb) run <<< "25"  # 单行输入
(gdb) run < input.txt  # 文件重定向

# 方式三，直接输入
```

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

## Valgrind

### 基础使用

#### 1. 编译程序
添加调试符号（`-g` 参数）：
~~~bash
gcc -g -o my_program my_program.c
~~~

#### 2. 基本命令
~~~bash
valgrind --leak-check=full ./my_program
~~~

#### 3. 常用参数
| 参数 | 说明 |
|------|------|
| `--leak-check=full` | 显示详细内存泄漏信息 |
| `--track-origins=yes` | 追踪未初始化值来源 |
| `--show-reachable=yes` | 显示可达的内存泄漏 |
| `--log-file=filename` | 输出到指定文件 |

---

### 检测示例

#### 示例代码（buggy.c）
~~~c
#include <stdlib.h>

void leak() {
    malloc(1024); // 内存泄漏
}

int main() {
    int arr[5];
    arr[5] = 0;   // 越界访问
    leak();
    return 0;
}
~~~

#### 运行检测
~~~bash
gcc -g -o buggy buggy.c
valgrind --leak-check=full ./buggy
~~~
### 关键输出解读
```shell
==12345== Invalid write of size 4
==12345== at 0x804843F: main (buggy.c:9) # 越界访问位置

==12345== 1,024 bytes in 1 blocks are definitely lost
==12345== at 0x402A17C: malloc (vg_replace_malloc.c:381)
==12345== by 0x8048435: leak (buggy.c:4) # 内存泄漏位置
```
---

### 常见内存问题类型
1. **内存泄漏**
   - Definitely lost：明确泄漏
   - Indirectly lost：间接泄漏
   - Possibly lost：可能的泄漏

2. **非法内存访问**
   - 读取未初始化内存
   - 数组越界访问
   - 使用已释放内存

---

### 高级技巧
1. 排除第三方库干扰：
~~~bash
valgrind --suppressions=suppress.txt ./my_program
~~~

2. 结合 GDB 调试：
~~~bash
valgrind --vgdb=yes --vgdb-error=0 ./my_program
~~~

3. 性能分析：
~~~bash
valgrind --tool=cachegrind ./my_program
~~~

---
