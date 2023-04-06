## GDB入门

所谓调试（Debug），就是让代码一步一步慢慢执行，跟踪程序的运行过程。比如，可以让程序停在某个地方，查看当前所有变量的值，或者内存中的数据；也可以让程序一次只执行一条或者几条语句，看看程序到底执行了哪些代码。

也就是说，通过调试程序，我们可以监控程序执行的每一个细节，包括变量的值、函数的调用过程、内存中数据、线程的调度等，从而发现隐藏的错误或者低效的代码。

就好像编译程序需要借助专业的编译器，调试程序也需要借助专业的辅助工具，即调试器（Debugger）。

GNU调试器GDB代表从具有命令行接口的命令行解释器（command shell）运行的调试器。

接下来，我们通过调试一个简单的、可预测的示例来熟悉GDB及其基本特性。


### 准备一个简单的示例

我们以计算任意非负整数$n$的阶乘函数$n!$为例，$0!=1$，当$n>0$时，$n! = n \times (n-1)!$。

下面给出计算阶乘函数的程序factorial的源代码。

```c
/* factorial.c */
/* Copyright (c) 2008 Thorsten Groetker, Ulrich Holtmann, Holger Keding, 
Markus Wloka. All rights reserved. Your use or disclosure of this source code 
is subject to the terms and conditions of the end user license agreement (EULA)
that is part of this software package. */

#include <stdio.h>
#include <stdlib.h>

int factorial(int n) {
    int result = 1;
    if(n == 0) 
        return result;
    result = factorial(n-1) * n;
    return result;
}

int main(int argc, char **argv) {
    int n, result;
    if(argc != 2) {
        fprintf(stderr, "usage: factorial n, n >=0\n");
        return 1;
    }
    n = atoi(argv[1]);
    result = factorial(n);
    printf("factorial %d!=%d\n", n, result);
    return 0;
}
```

这里将函数factorial(int n)实现为它自己的一个递归调用。注意，代码的安全性很差，因为没有$n$的负值保护，
也没有由于$n>12$时32位整数溢出而导致错误结果的保护。本例就是为了演示用来查找这些bug的基本调试器特性。


### 使调试器与程序一切运行

对于GNU编译器GCC和大部分其他编译器，进行调试的编译器标志是`-g`。
用GCC编译factorial程序的方法如下:
```
$ gcc -g -o factorial factorial.c
```

对于GDB调试器，需要健入命令gdb，并输入程序名称作为它的第一个参数。
运行程序的命令是run，后跟要传递给程序的命令行参数。例如:
```
$ gdb factorial
<lots of copyright stuff..>
(gdb) run 1
Starting program: /home/hexu/git/C.And.Cpp.Debugging.Tutorial/gdb/code/c/factorial/factorial 1
factorial 1!=1
[Inferior 1 (process 6631) exited normally]
(gdb)
```

后面将详细介绍断点，但现在这种方法足以使我们看到程序的输出：
```
factorial 1!=1
```

此时，就可以用调试器来做一些有用的测试了。用参数$n=-1$来运行程序，这将导致递归不能停止：
```
(gdb) run -1
Starting program: /home/hexu/git/C.And.Cpp.Debugging.Tutorial/gdb/code/c/factorial/factorial -1

Program received signal SIGSEGV, Segmentation fault.
0x0000555555554722 in factorial (n=<error reading variable: Cannot access memory at address 0x7fffff7feffc>) at factorial.c:10
10      int factorial(int n) {
(gdb)
```

上面的GDB输出告诉我们程序在factorial函数的第14行发生了一个内存错误，原因是它尝试再次调用它自己时栈溢出了。
GDB显示了发生错误的实际源代码行。


### 学习在程序崩溃时执行栈跟踪

栈跟踪是用于调试崩溃位置的有用信息，因为它可以告诉我们导致崩溃的函数调用链。

GDB调试器通过编号来引用栈帧，其中当前栈帧的编号为0，main()函数的栈帧编号最高。
GDB的栈跟踪命令是`bt`，`backtrace`或`where`。栈跟踪确定本例中的错误是程序的栈溢出，
原因是对factorial()函数进行了过多的递归调用：
```
(gdb) run -1
Starting program: /home/hexu/git/C.And.Cpp.Debugging.Tutorial/gdb/code/c/factorial/factorial -1

Program received signal SIGSEGV, Segmentation fault.
0x0000555555554722 in factorial (n=<error reading variable: Cannot access memory at address 0x7fffff7feffc>) at factorial.c:10
10      int factorial(int n) {
(gdb) backtrace
#0  0x0000555555554722 in factorial (n=<error reading variable: Cannot access memory at address 0x7fffff7feffc>) at factorial.c:10
#1  0x0000555555554744 in factorial (n=-174653) at factorial.c:14
#2  0x0000555555554744 in factorial (n=-174652) at factorial.c:14
<lots of frames..>
#174652 0x0000555555554744 in factorial (n=-2) at factorial.c:14
#174653 0x0000555555554744 in factorial (n=-1) at factorial.c:14
#174654 0x00005555555547b0 in main (argc=2, argv=0x7fffffffdc98) at factorial.c:25
```

可以在调用栈中进行上下导航，并检查函数参数和局部变量的值。在GDB中，可以使用命令`up`或`down`在栈中移动。


### 学习使用断点

调试器提供了一系列断点命令，如下:

- 行断点（Line breakpoint）-- 当到达源代码中的指定行时，暂停程序。
- 函数断点（Function breakpoint）-- 当到达指定函数的第一行时，暂停程序。
- 条件断点（Conditional breakpoint）-- 如果特定条件保持为真，则暂停程序。
- 事件断点（Event breakpoint）-- 当发生特定事件时，使程序进入暂停模式。支持的事件包括来自操作系统的signals和C++异常（exception）。

在GDB调试器中对程序打断点，最常用的就是`break`命令。


### 学习在程序中导航

调试器有一些用于运行和暂停程序的命令。

- run -- run命令将开始程序。可以通过命令行参数或环境变量来控制和更改运行程序的环境。
- start -- start命令将运行程序，直到main()的第一行，然后停止程序执行。这样就不必搜索包含main()函数的文件，并在其第一行设置显式的断点了。
- pause -- pause命令将中断一个正在运行的程序。在GDB中，键入Ctrl-C将起到相同的效果。
- continue -- 调试器命令continue使暂停的程序恢复执行。

调试器提供了逐行步进（stepping）的功能。有3种不同的步进模式，即step-into、step-over和step-out，它们的区别在于处理函数调用的方式。
下面将介绍它们的工作原理。
- step-into -- 调试器命令step-into（在GDB中是`step`）的作用是移动到下一个可执行的代码行。如果当前行是一个函数调用，则调试器将进入函数，并停止在函数体的第一行。
- step-over -- 调试器命令step-over（在GDB中是`next`）的作用是在同一个调用栈层中移动到下一个可执行的代码行。如果当前行是一个函数调用，则调试器将在函数调用之后的下一条语句停止。调试器不会进入函数体。如果当前行是函数的最后一行，则step-over将进入下一个栈层，并在调用函数的下一行停止。
- step-out -- 调试器命令step-out（在GDB中是`finish`）的作用是在栈中进入下一个栈层，并在调用函数的下一行停止。


### 学习检查数据：变量和表达式

显示变量值的方法有：

- print -- print命令将输出变量或表达式的当前值。
- display expression -- 持续显示表达式的值。每次程序暂停执行时，值都会更新。

例如，在factorial示例中的第11行设置一个断点，然后启动程序。
当程序在第11行暂停时，在GDB中键入print n。
```
(gdb) list
6
7       #include <stdio.h>
8       #include <stdlib.h>
9
10      int factorial(int n) {
11          int result = 1;
12          if(n == 0)
13              return result;
14          result = factorial(n-1) * n;
15          return result;
(gdb) break 11
Breakpoint 1 at 0x725: file factorial.c, line 11.
(gdb) run 12
Starting program: /home/hexu/git/C.And.Cpp.Debugging.Tutorial/gdb/code/c/factorial/factorial 12

Breakpoint 1, factorial (n=12) at factorial.c:11
11          int result = 1;
(gdb) print n
$1 = 12
(gdb)
```

如果必须分析和反复输出相同的表达式，则可以在调试器每次暂停时，使用显示（display）特性来计算并自动输出表达式的结果。
使用显示特性可以在调试器中对程序进行导航的同时有效的监视感兴趣的变量。在GDB中，命令名称是`display`。


### 一个简单示例的调试会话

最后，我们通过一个简单示例的完整调试过程，来回顾一下前面介绍的调试器命令和特性。以factorial为例，用调试器来查明为什么
当$n>=13$时，程序返回错误值。程序计算$13!$的返回结果是$1932053504$，而正确的值是$13!=6227020800$。

第一项任务是用参数$13$来运行程序，并在main()的第一行停止，然后逐行前进，直到到达第25行，即factorial()函数被调用的地方。
这样做的目的是检查命令行参数是否被正常解析，以及整数值$13$是否被传递给factorial()函数。然后进入factorial()函数。
用GDB来调试这个示例：
```
$ gdb factorial
(gdb) start 13
Temporary breakpoint 1 at 0x763: file factorial.c, line 20.
Starting program: /home/hexu/git/C.And.Cpp.Debugging.Tutorial/gdb/code/c/factorial/factorial 13

Temporary breakpoint 1, main (argc=2, argv=0x7fffffffdc68) at factorial.c:20
20          if(argc != 2) {
(gdb) next
24          n = atoi(argv[1]);
(gdb) next
25          result = factorial(n);
(gdb) print n
$1 = 13
(gdb) step
factorial (n=13) at factorial.c:11
11          int result = 1;
(gdb)
```

我们需要检查递归调用是否正常工作，因此在第13行设置一个断点，以便检查当$n=0$时，函数不再调用自己。
另外在第15行设置一个断点，以便能够查看每次调用factorial的返回值。这里不使用步进，而是使程序继续到第一个断点。
```
(gdb) list
6
7       #include <stdio.h>
8       #include <stdlib.h>
9
10      int factorial(int n) {
11          int result = 1;
12          if(n == 0)
13              return result;
14          result = factorial(n-1) * n;
15          return result;
(gdb) break 13
Breakpoint 2 at 0x555555554732: file factorial.c, line 13.
(gdb) break 15
Breakpoint 3 at 0x55555555474f: file factorial.c, line 15.
(gdb) continue
Continuing.

Breakpoint 2, factorial (n=0) at factorial.c:13
13              return result;
(gdb)
```

输出变量n和result，检查函数是否正确工作。还可以使用变量result上的显示命令，这样就不必重复键入print命令。
```
(gdb) print n
$1 = 0
(gdb) print result
$2 = 1
(gdb) display result
1: result = 1
(gdb)
```

当继续重复continue命令时，得到当$n=0, 1, \cdots, 13$时程序第15行的变量result的输出结果。
当$n=13$时，程序暂停。
```
(gdb) continue
Continuing.

Breakpoint 3, factorial (n=1) at factorial.c:15
15          return result;
1: result = 1
(gdb) continue
...
Breakpoint 3, factorial (n=12) at factorial.c:15
15          return result;
1: result = 479001600
(gdb) continue
Continuing.

Breakpoint 3, factorial (n=13) at factorial.c:15
15          return result;
1: result = 1932053504
(gdb) print 13 * 479001600
$3 = 1932053504
```

注意，$n=12$时的结果仍然是正确的，但它与$13$相乘得到了一个错误结果。重复第14行的计算，
输出表达式13*479001600，看到它再次产生计算错误。错误原因是$13!$的值太大了，无法存储在
int类型的32位变量中，因此整数溢出导致了错误的结果。

我们把其中一个整数转化成long类型在计算，就能看到正确的结果。
```
(gdb) print ((long) 13) * 479001600
$6 = 6227020800
(gdb)
```

至此，GDB入门介绍结束，其中90%的内容都来自于《软件调试实战》这本书的第3章，有兴趣的可以去看看原书，里面不止有GDB的介绍。

### 附录：调试命令一览

| 命令名称                | GDB语法                 |
| ----------------------- | ----------------------- |
| 运行程序                | run [_args_]            |
| 启动程序                | start [_args_]          |
| 暂停                    | **Ctrl-C**              |
| 继续运行                | cont                    |
|                         |                         |
| step-over               | next                    |
| step-into               | step                    |
| step-out                | finish                  |
|                         |                         |
| 断点                    | break _file:lineno_     |
| 跟踪点                  | watch _file:lineno_     |
| 观察点                  | watch _expr_            |
|                         |                         |
| 栈跟踪                  | bt, where               |
| 输出表达式              | print _expr_            |
| 显示表达式              | display _expr_          |
|                         |                         |
| 设置程序参数            | set args _arg1_ _[...]_ |
| 设置变量                | set var _var=expr_      |
| 设置环境变量            | set env _var[=val]_     |
|                         |                         |
| 显示机器码              | disassemble             |
| 在机器码中执行step-over | nexti                   |
| 在机器码中执行step-into | stepi                   |
|                         |                         |
| 条件断点                | condition _bnum_        |
| 事件断点                | handle, signal          |
| 异常断点                | catch, throw            |
| 函数断点                | break _function_        |
| 临时断点                | tbreak                  |
| 列出所有断点            | info breakpoints        |
|                         |                         |
| 将命令连接到断点        | commands _bnum_         |
| 输出到命令行            | printf                  |
|                         |                         |
| 查找函数                | info functions _expr_   |
| 调用函数                | call _expr_             |
| 修改函数返回值          | return _expr_           |
|                         |                         |
| 输出类型                | whatis _arg_            |
| 输出类型描述            | ptype _arg_             |
| 输出内存内容            | x _arg_                 |
| 选择栈帧                | frame _arg_             |
| 输出帧描述              | info frame              |


### 参考资料:
- 《The Developer’s Guide to Debugging》:  3 Getting to the Root – Source Code Debuggers
