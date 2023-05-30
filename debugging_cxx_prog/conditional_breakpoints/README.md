### 条件断点

我们以STR类为例，我们将使用GDB的condition命令创建一个条件断点，  
以便在用字符串字面值"def"调用的地方停止。

```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb break_str
(gdb) break break_str.cc:8
Breakpoint 1 at 0x157d: file break_str.cc, line 8.
(gdb) condition 1 a[0] == 'd' && a[1] == 'e' && a[2] == 'f' && a[3] == 0
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str/break_str
warning: Error disabling address space randomization: Operation not permitted
2: abc

Breakpoint 1, STR::STR (this=0x7ffc200646c0, a=0x562ae979900d "def") at break_str.cc:8
8               s=a;
(gdb)
```

在GDB中创建条件断点的另一种方法是使用一个常规断点，然后附加一个包含条件语句的命令：

```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb break_str
(gdb) break 8
Breakpoint 1 at 0x157d: file break_str.cc, line 8.
(gdb) commands 1
Type commands for breakpoint(s) 1, one per line.
End with a line saying just "end".
> silent
> print strcmp(a, "def")
> if $$0 == 0
 >    print "FOUND the right BP!"
 > else
 >    printf "STR CTOR with a=0x%x \"%s\"\n", a, a
 >    continue
 > end
> end
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str/break_str
warning: Error disabling address space randomization: Operation not permitted
$1 = -3
STR CTOR with a=0xb9fff009 "abc"
2: abc
$2 = 0
$3 = "FOUND the right BP!"
(gdb) bt
#0  STR::STR (this=0x7ffcd48d48b0, a=0x558fb9fff00d "def") at break_str.cc:8
#1  0x0000558fb9ffe417 in main (argc=1, argv=0x7ffcd48d4a08) at break_str.cc:47
(gdb)
```

表达式`$$0`已用前一行返回的结果，在本例中就是print strcmp(a, "def")命令。

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.4 Conditional Breakpoints and Breakpoint Commands
