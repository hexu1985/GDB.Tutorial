### 使用观察点

观察点或数据断点的作用是在表达式值发生变化时停止程序的执行。  
在GDB中，watch命令的表达式可以是变量、内存地址或任意的复杂表达式。  
调试器将不间断地监视表达式，并在表达式值发生更改的语句停止程序的执行。

我们以STR类程序为例。目的是找到变量x.num发生更改的所有地发。  
每次x.num更改时，调试器都应该停止，并输出哪条语句更改了它。


```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb break_str
(gdb) start
Temporary breakpoint 1 at 0x139a: file break_str.cc, line 44.
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str/break_str 
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 1, main (argc=21959, argv=0x55c7ab508158 <std::__ioinit>) at break_str.cc:44
44      int main(int argc, char* argv[]) {
(gdb) watch x.num
Hardware watchpoint 2: x.num
(gdb) cont
Continuing.

Hardware watchpoint 2: x.num

Old value = 0
New value = 1
STR::STR (this=0x7ffdae237200, a=0x55c7ab506009 "abc") at break_str.cc:10
10          }
(gdb) where
#0  STR::STR (this=0x7ffdae237200, a=0x55c7ab506009 "abc") at break_str.cc:10
#1  0x000055c7ab5053d9 in main (argc=1, argv=0x7ffdae237378) at break_str.cc:45
```

注意，如果在局部变量中设置了观察点，则当程序离开这个局部范围时，GDB将删除这些观察点。  
例如，如果在一个构造函数中停止，并在num上设置观察点，则一旦离开此构造函数，该观察点将丢失。

```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb break_str
(gdb) start
Temporary breakpoint 1 at 0x139a: file break_str.cc, line 44.
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str/break_str
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 1, main (argc=21939, argv=0x55b3f29fd158 <std::__ioinit>) at break_str.cc:44
44      int main(int argc, char* argv[]) {
(gdb) break 8
Breakpoint 2 at 0x55b3f29fa57d: file break_str.cc, line 8.
(gdb) cont
Continuing.

Breakpoint 2, STR::STR (this=0x7ffd15215190, a=0x55b3f29fb009 "abc") at break_str.cc:8
8               s=a;
(gdb) watch num
Hardware watchpoint 3: num
(gdb) cont
Continuing.

Hardware watchpoint 3: num

Old value = 0
New value = 1
STR::STR (this=0x7ffd15215190, a=0x55b3f29fb009 "abc") at break_str.cc:10
10          }
(gdb) cont
Continuing.

Hardware watchpoint 3: num

Old value = 1
New value = 2
STR::STR (this=0x7ffd152151d0, a=...) at break_str.cc:14
14          }
(gdb) cont
Continuing.
Error evaluating expression for watchpoint 3
current stack frame does not contain a variable named `this'
Watchpoint 3 deleted.
show (z=...) at break_str.cc:41
41          std::cout << z.num_objs() << ": " << z.c_str() << std::endl;
(gdb)
```

在GDB中，可以在需要观察的变量的地址上设置观察点。这个表达式适用于整个程序，  
因此观察点不会被删除：


```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb break_str
(gdb) start
Temporary breakpoint 1 at 0x139a: file break_str.cc, line 44.
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str/break_str
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 1, main (argc=21861, argv=0x55653fa99158 <std::__ioinit>) at break_str.cc:44
44      int main(int argc, char* argv[]) {
(gdb) break 8
Breakpoint 2 at 0x55653fa9657d: file break_str.cc, line 8.
(gdb) cont
Continuing.

Breakpoint 2, STR::STR (this=0x7ffd1ae581c0, a=0x55653fa97009 "abc") at break_str.cc:8
8               s=a;
(gdb) p &num
$1 = (int *) 0x55653fa99154 <STR::num>
(gdb) watch *0x55653fa99154
Hardware watchpoint 3: *0x55653fa99154
(gdb) cont
Continuing.

Hardware watchpoint 3: *0x55653fa99154

Old value = 0
New value = 1
STR::STR (this=0x7ffd1ae581c0, a=0x55653fa97009 "abc") at break_str.cc:10
10          }
(gdb) cont
Continuing.

Hardware watchpoint 3: *0x55653fa99154

Old value = 1
New value = 2
STR::STR (this=0x7ffd1ae58200, a=...) at break_str.cc:14
14          }
(gdb) cont
Continuing.
2: abc

Hardware watchpoint 3: *0x55653fa99154

Old value = 2
New value = 1
STR::~STR (this=0x7ffd1ae58200, __in_chrg=<optimized out>) at break_str.cc:15
15          ~STR() {
(gdb)
```

不要忘记在地址前面加`*`。如果不小心键入了`watch 0x55653fa99154`，  
而不是`watch *0x55653fa99154`，那么观察点将检查一个十六进制的常量地址值，  
它永远不会发生变化。

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.6 Using Watchpoints
