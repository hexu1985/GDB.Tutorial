### 条件断点

我们以STR类为例，我们将使用GDB的condition命令创建一个条件断点，  
以便在用字符串字面值"def"调用的地方停止。

```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb break_str
(gdb) break break_str.cc:7
Breakpoint 1 at 0x10bd: file break_str.cc, line 7.
(gdb) condition 1 a[0] == 'd' && a[1] == 'e' && a[2] == 'f' && a[3] == 0
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str/break_str
2: abc

Breakpoint 1, STR::STR (this=0x7fffffffdc80, a=0x55555555537d "def") at break_str.cc:7
7	    STR(const char* a) {
(gdb)
```

在GDB中创建条件断点的另一种方法是使用一个常规断点，然后附加一个包含条件语句的命令：

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.4 Conditional Breakpoints and Breakpoint Commands
