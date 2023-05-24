### 断点命令

同调试C代码一样，可以为断点设置一系列命令来让断点中断时执行之。例如，你可能想要打印某些表达式的值，或者激活别的断点。  
使用commands命令设置命令列表。
```
commands breakpoint-number
...
commands
...
end
```
commands 后面接命令行。最后输入一行 end 来结束命令。  

我们以STR类为例，该类有一个用于字符串字面值的STR构造函数。我们使用断点命令来找到构造函数  
被调用的时间和位置。每个构造函数调用都输出一行，然后程序自动继续执行。

```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb break_str
(gdb) break break_str.cc:7
Breakpoint 1 at 0x10bd: file break_str.cc, line 7.
(gdb) commands 1
Type commands for breakpoint(s) 1, one per line.
End with a line saying just "end".
> silent
> printf "STR CTOR with a=0x%x \"%s\"\n", a, a
> continue
> end
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str/break_str
STR CTOR with a=0x55555379 "abc"
2: abc
STR CTOR with a=0x5555537d "def"
STR CTOR with a=0x55769280 "abcdefabcdefabcdef"
3: abcdefabcdefabcdef
[Inferior 1 (process 28173) exited normally]
(gdb)
```

上例中，我们使用commands命令指定一个命令序列，每次到达一个特定断点时，都执行此序列。  
这个命令序列的第一条命令是silent，它的作用是不输出后续命令的执行结果，以免屏幕输出过多。  
我们希望只输出一行，并使用GDB的printf命令来创建整齐的格式化输出。continue命令恢复程序的执行。  
次命令序列以end命令终止。

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.4 Conditional Breakpoints and Breakpoint Commands
