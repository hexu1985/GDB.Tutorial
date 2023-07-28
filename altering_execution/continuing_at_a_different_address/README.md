### 在不同的位置上继续执行

通常，继续执行程序时，用 continue 命令中断的地方上继续执行。不过，用下列命令，可以在自己选定的位置上继续执行程序：
- jump linespec
- jump location
在给定的行 linespec 或者 location 上继续执行。如果那个位置上有断点的话，执行会立即中断。

jump 命令除了改变程序计数器之外，不会改变当前堆栈帧，堆栈指针，也不改变任何内存位置和任何寄存器。
如果行 linespec 是在当前执行的函数之外的函数里，如果这两个函数的参数模式或者本地变量不一样的话，
那么结果就可能很诡异。由于这个原因，如果指定的行不在当前执行的函数里的话，jump 命令需要用户确认。
不过，如果用户很熟悉程序的机器语言的话，这个诡异的结构也是可以预测的。
在很多系统里，把一个新的值存入寄存器$pc 里和 jump 命令的效果一样。不同之处在于，
这样做不会让程序开始执行；这样做只会改变程序继续执行的地址。

最常使用 jump 命令的场景在于回退已经执行过的程序–中间可能有几个断点，可以更详
细的检查程序的执行。

我们以capitalize程序为例，我们将使用此命令跳过第24行一次，从而避免修改change_word(...)调用：

```
$ gcc -g3 -Wall -Wextra -o capitalize capitalize.c
$ gdb capitalize
(gdb) set args "Foofoo, foobar and Bar!" foo bar
(gdb) tbreak 24
Temporary breakpoint 1 at 0x87b: file capitalize.c, line 24.
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/c/capitalize/capitalize "Foofoo, foobar and Bar!" foo bar

Temporary breakpoint 1, capitalize_str (str=0x555555756260 "Foofoo, foobar and Bar!", name=0x7fffffffe0c5 "foo") at capitalize.c:24
24                   change_word( str+n, len_name );
(gdb) jump 25
Continuing at 0x555555554895.
Total 4 hits:
original: Foofoo, foobar and Bar!
modified: Foofoo, FOOBAR and BAR!
[Inferior 1 (process 8276) exited normally]
(gdb)
```

对比正常的执行结果：

```
$ ./capitalize "Foofoo, foobar and Bar!" foo bar
Total 4 hits:
original: Foofoo, foobar and Bar!
modified: FOOfoo, FOOBAR and BAR!
```

可以发现，GDB调试时，修改后的字符串现在以"Foofoo..."开头，而不是"FOOfoo..."，因此jump命令有效地跳过了第24行地函数调用change_word(...)。
