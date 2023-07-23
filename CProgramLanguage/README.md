# README of this project

*** 课程成绩: A+ ***

这是燕山大学大一上C语言结课项目的源代码。

该代码完全体具有以下特性

* 以链表存储图书信息
* 从数据文件加载图书馆
* 保存图书馆到数据文件
* 从宏定义不同的程序生成的数据文件中正确的读取图书馆
* 实现有限范围内的增改删查
* 部分跨平台

### 什么叫“宏定义不同的程序生成的数据文件”？

程序首部有4个`define`语句，它们管理着下面所述的四个字符串长度限制

更改这些`define`语句后，编译出来的程序可以兼容之前的程序生成的数据文件。

不过，依`define`语句修改情况，可能会出现字符串的截断，导致数据丢失。

## 数据文件结构

从首部开始：

1. 4个int大小的数据，分别是 图书名称字段长度B 图书作者字段长度A 图书ISBN字段长度I 图书馆名称字段长度L
2. 一个长度为L的以\0结尾的字符串，为图书馆名称
3. 一个int大小的数据，为书籍数量n
4. n个大小为(B+A+I+2*long long)的结构体

图书结构体的定义如下：

```c
typedef struct {
    char name[B];
    char author[A];
    char isbn[I];
    long long addAt;
    long long updatedAt;
} Book;
```

数据文件的结构体定义如下：

```c
#pragma pack(1)

typedef struct {
    int B;
    int A;
    int I;
    int L;
    char library_name[L];
    int n;
    Book bookList[n];
} DataFile;
```

## Bugs

* Linux系统下的原生编译的程序会出现输入缓冲区清空失败的问题

## 提醒

报告PDF仅供参考，请勿抄袭

上传的源代码与报告中附录B中的源代码并不一致，请仔细甄别。
