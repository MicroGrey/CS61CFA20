# Lab 02
## Makefile

### 什么是 Makefile？
**Makefile** 是一个文本文件，用于自动化代码编译、测试和清理等任务。它通过定义**规则**告诉系统：
- **要生成什么文件**（如可执行程序）
- **依赖哪些文件**（如 `.c` 源文件）
- **如何生成它们**（如用 `gcc` 编译）

类似烹饪食谱：列出食材（依赖）、步骤（命令），最终做出菜品（目标文件）。

---

### 为什么需要 Makefile？
假设你有一个包含多个源文件的项目：
- 手动编译：每次修改后需重复输入冗长的 `gcc` 命令。
- **Makefile 的优势**：
  - **自动化**：输入 `make` 一键完成所有编译步骤。
  - **智能更新**：只重新编译修改过的文件，节省时间。
  - **统一管理**：所有编译规则集中在一个文件中。

---

### 1. 基础规则
```makefile
# 规则格式：
目标文件: 依赖文件
    编译命令  # 必须用Tab缩进

# 真实案例：编译 hello.c
hello: hello.c
    gcc hello.c -o hello
```
**运行：**
```bash
make   # 自动编译
./hello
```

### 2. 多文件编译
```makefile
# 定义变量（可选）
CC = gcc
CFLAGS = -Wall

# 最终目标
app: main.o utils.o
    $(CC) main.o utils.o -o app

# 生成中间文件
main.o: main.c
    $(CC) -c main.c

utils.o: utils.c
    $(CC) -c utils.c

# 清理命令
clean:
    rm -f *.o app
```
**使用：**
```bash
make     # 编译所有文件
make clean  # 删除生成的文件
```

### 3. 常用技巧
#### (1) 自动变量
```makefile
app: main.c
    $(CC) $< -o $@  
    # $< = 第一个依赖文件（main.c）
    # $@ = 目标文件名（app）
```

#### (2) 伪目标声明
```makefile
.PHONY: clean  # 防止有同名文件clean规则时失效
clean:
    rm -f *.o app
```

#### (3) 通用模式规则
```makefile
%.o: %.c   # 所有.c文件自动生成.o
    $(CC) -c $<
```

#### (4) 默认目标声明
```makefile
# 声明 all 为第一个目标（默认执行），项目构建的入口点
all: app1 app2  # 同时构建 app1 和 app2

app1: main1.c
    $(CC) $< -o $@

app2: main2.c
    $(CC) $< -o $@

.PHONY: all  # 声明为伪目标，即使存在同名文件也不会影响规则执行
```

### 4. 常见错误急救
| 错误现象                  | 解决方法                     |
|--------------------------|----------------------------|
| `missing separator`       | 命令前必须用Tab，不能用空格   |
| `Nothing to be done`      | 目标文件已存在，运行`make clean` |
| `No rule to make target`  | 检查依赖文件名是否拼写错误     |

### 5. 即用模板
#### 单文件项目
```makefile
output: input.c
    gcc input.c -o output

clean:
    rm -f output
```

#### 多文件项目
```makefile
TARGET = myapp
SOURCES = main.c utils.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
    gcc $^ -o $@

%.o: %.c
    gcc -c $<

clean:
    rm -f $(OBJECTS) $(TARGET)
```

### 行动：
1. 新建 `Makefile` 文件（无后缀）
2. 复制上方模板
3. 终端执行 `make`