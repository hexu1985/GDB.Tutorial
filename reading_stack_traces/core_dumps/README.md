### 核心转储

如果程序遇到bug并触发了一个分段错误，则操作系统将创建一个核心转储文件（core dump file）。
核心转储为事后调试提供了数据，并且可用来查找程序失败的原因。

以下程序将引发一个分段错误：

```c
  1 /* answer.c */
  2 /* Copyright (c) 2008 Thorsten Groetker, Ulrich Holtmann, Holger Keding,
  3 Markus Wloka. All rights reserved. Your use or disclosure of this source code
  4 is subject to the terms and conditions of the end user license agreement (EULA)
  5 that is part of this software package. */
  6
  7 #include <stdio.h>
  8
  9 void runexperiment() {
 10     int *answerp;
 11     answerp = (void *) 42;
 12     printf("The answer is %d\n", *answerp);
 13 }
 14
 15 void createplanet() {
 16     runexperiment();
 17 }
 18
 19 int main() {
 20     printf("Hello Universe! Computing answer ...\n");
 21     fflush(stdout);
 22     createplanet();
 23     return 0;
 24 }
```

运行程序，第12行将发生分段错误：

```
$ gcc -g3 -Wall -Wextra -o answer answer.c
$ ./answer
Hello Universe! Computing answer ...
段错误 (核心已转储)
```

我们使用gdb myprog core命令将核心转储和程序一并载入调试器。

```
$ gdb answer /corefile/core-answer-29251-1697617423
Reading symbols from answer...done.

warning: exec file is newer than core file.
[New LWP 29251]
Core was generated by `./answer'.
Program terminated with signal SIGSEGV, Segmentation fault.
#0  0x000055afa540272e in runexperiment () at answer.c:12
12	    printf("The answer is %d\n", *answerp);
(gdb) where
#0  0x000055afa540272e in runexperiment () at answer.c:12
#1  0x000055afa5402754 in createplanet () at answer.c:16
#2  0x000055afa5402780 in main () at answer.c:22
(gdb)
```

核心转储包含程序已分配的所有数据的一个副本。栈数据也是其中的一部分，
利用栈数据可以在程序终止后使用调试器来查看栈跟踪。

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.9.7 Core Dumps