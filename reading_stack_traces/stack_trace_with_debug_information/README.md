### 带调试信息的栈跟踪

栈跟踪（回溯）是关于程序如何达到所处位置的概要。每行显示一个帧，对于多个帧的情况，
从当前执行的帧（0 帧）开始，接下来是它的调用者（1 帧），以此类推。

GDB中栈跟踪的命令包括：

- backtrace 
    bt 打印整个堆栈的回溯：每帧一行。

可以用输入系统中断字符在任意时间中断回溯，通常是 Ctrl-c.

- backtrace n
    bt n 类似的，但只打印最内 n 层帧。

- backtrace full
    bt full
    bt full n
    bt full -n 也打印本地变量。n 是要打印的帧的数量，如上所述

where 和 info stack 是 backtrace 的别名。


这里仍以STR类程序为例。修改第22行中的operator+代码，使其引用一个空指针，
从而引起一个段错误：

```
file break_str.cc
 21     const STR& operator+ (const STR& a) {
 22         int* dummy=0;
 23         (*dummy)++;             // <== BUG
 24         s += a.s; return *this;
 25     }
```

编译带调试信息的程序，然后运行它，直到崩溃：

```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb ./break_str
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str_segment_fault/break_str
2: abc

Program received signal SIGSEGV, Segmentation fault.
0x00005555555551d0 in STR::operator+ (this=0x7fffffffdce0, a=...) at break_str.cc:23
23	        (*dummy)++;             // <== BUG
(gdb) where
#0  0x00005555555551d0 in STR::operator+ (this=0x7fffffffdce0, a=...) at break_str.cc:23
#1  0x0000555555554f93 in main (argc=1, argv=0x7fffffffde48) at break_str.cc:49
(gdb)
```

信号SIGSEGV是一个分段错误，当程序试图访问没有权限的内存地址时，就会发生此错误。
在本例中，我们访问了地址0x0，这是分段错误的一个常见原因。
其他致命的信号还有SIGBUS（非法总线地址）、SIGILL（非法指令）和SIGFPE（浮点异常）。

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.9.1 Stack Trace of Source Code Compiled with Debug Information