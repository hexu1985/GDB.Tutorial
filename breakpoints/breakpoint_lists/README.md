### GDB中的断点列表

当创建新断点时，GDB会告知你分配给该断点的编号。例如：

```
$ gcc -Wall -g3 -Wall -Wextra -o test1 test1.c
$ gdb test1
(gdb) break main
Breakpoint 1 at 0x5fe: file test1.c, line 4.
```

如果忘记了分配给哪个断点的编号是什么，可以使用info breakpoints命令来提示。

```
$ gcc -Wall -g3 -Wall -Wextra -o test1 test1.c
$ gdb test1
(gdb) break main
Breakpoint 1 at 0x5fe: file test1.c, line 4.
(gdb) info breakpoints
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x00000000000005fe in main at test1.c:4
```

打印断点信息的info breakpoints命令有两种形式：
```
info breakpoints [n]
info break [n]
```
可选参数 n 代表打印特定断点的信息。对于每个断点，打印下列信息：
- Breakpoint Numbers
- Type 断点，监视点，或捕获点
- Disposition 断点是否标记为禁用或删除
- Enabled or Disabled 用’y'标记激活断点，用’n'标记断点禁用。
- Address 程序里的断点位置，内存里的位置。对于一个挂起的断点，它的位置是未知的，这个域会包含’<PENDING>’。
    这类断点在共享库没被加载前世不会起作用的。一个断点对应多个位置的话，这个域会包含‘<MULTPLE>’。
- What 断点位置在程序源代码，文件和行号。对于一个挂起的断点，由于在对应的共享库未被加载前不能解释，
    此时断点命令会显示一个初始的字符串。 如果断点是有条件的，info break 会显示被断点影响的行上的条件；
    断点命令，如果有的话，会在这行后显示。一个挂起的断点可以有条件的指定。这个条件会在共享库加载后分析有效性，
    以此来确定一个有效的位置。


一个断点可能对应于多个位置。这种情况的例子如下：
- 对于 C++构造函数，GCC 编译器产生函数体多个的实例，用于不同的重载场景。
- 对于 C++模板函数，函数里一个给定的行可以对应于任意数量的实例。
- 对于内联函数，一个给定的源代码行可以对应于多个内联的地址。

在这些情况下，GDB 会在这些相关的位置插入断点。
一个对应于多个位置的断点可能会用多行来显示断点信息表–一个表头行，接下来是每一行对应于每一断点位置。
表头行在地址列里有’<MULTIPLE>’。每个位置有单独的行，这一行包含位置的实际地址，和那个位置对应的函数名。
断点号列的形式是断点号.位置号。
例如：
```
g++ -g3 -Wall -Wextra -o chapter_3.3 chapter_3.3.cpp -lpthread
(gdb) break test_fun
Breakpoint 1 at 0x16d7: test_fun. (4 locations)
(gdb) info breakpoints
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   <MULTIPLE>
1.1                         y     0x00000000000016d7 in test_fun(int) at chapter_3.3.cpp:76
1.2                         y     0x00000000000016fc in test_fun(char const*) at chapter_3.3.cpp:80
1.3                         y     0x0000000000002246 in test_1::test_fun() at chapter_3.3.cpp:52
1.4                         y     0x00000000000022fe in test_2::test_fun() at chapter_3.3.cpp:70
```


### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 2.3.1 Breakpoint Lists in GDB
- 《Debugging With GDB》:5.1.1 Setting Breakpoints
