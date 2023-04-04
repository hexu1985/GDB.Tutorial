### 断点命令列表

可以为断点设置一系列命令来让断点中断时执行之。例如，你可能想要打印某些表达式的值，或者激活别的断点。  
使用commands命令设置命令列表。
```
commands breakpoint-number
...
commands
...
end
```
commands 后面接命令行。最后输入一行 end 来结束命令。  

要删除一个断点的命令，输入 commands 后接一行 end；就是说，不带命令。

不带 `breakpoint-number` 参数的话，commands 指定最后的断点（不是最近执行到的断点）。

```
$ gcc -g3 -Wall -Wextra -o fibonacci fibonacci.c
$ gdb fibonacci
(gdb) break fibonacci
Breakpoint 1 at 0x67e: file fibonacci.c, line 13.
(gdb) commands 1
Type commands for breakpoint(s) 1, one per line.
End with a line saying just "end".
>printf "fibonacci was passed %d.\n", n
>end
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/c/fibonacci/fibonacci

Breakpoint 1, fibonacci (n=3) at fibonacci.c:13
13              if ( n <= 0 || n == 1 )
fibonacci was passed 3.
(gdb) continue
Continuing.

Breakpoint 1, fibonacci (n=2) at fibonacci.c:13
13              if ( n <= 0 || n == 1 )
fibonacci was passed 2.
(gdb) continue
Continuing.

Breakpoint 1, fibonacci (n=1) at fibonacci.c:13
13              if ( n <= 0 || n == 1 )
fibonacci was passed 1.
(gdb) continue
Continuing.

Breakpoint 1, fibonacci (n=0) at fibonacci.c:13
13              if ( n <= 0 || n == 1 )
fibonacci was passed 0.
(gdb) continue
Continuing.

Breakpoint 1, fibonacci (n=1) at fibonacci.c:13
13              if ( n <= 0 || n == 1 )
fibonacci was passed 1.
(gdb) continue
Continuing.
Fibonacci(3) is 3.
[Inferior 1 (process 13661) exited normally]
(gdb)
```

如果你指定的命令列表的的哥命令是 silent，通常断点中断的所产生的消息就不会打印。  
这个在打印一个特定的消息，然后接着继续执行的中断而言是可取的。  
silent 只在断点命令开头有意义。

```
$ gcc -g3 -Wall -Wextra -o fibonacci fibonacci.c
$ gdb fibonacci
(gdb) break fibonacci
Breakpoint 1 at 0x67e: file fibonacci.c, line 13.
(gdb) commands 1
Type commands for breakpoint(s) 1, one per line.
End with a line saying just "end".
>silent
>printf "fibonacci was passed %d.\n", n
>end
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/c/fibonacci/fibonacci
fibonacci was passed 3.
(gdb) continue
Continuing.
fibonacci was passed 2.
(gdb) continue
Continuing.
fibonacci was passed 1.
(gdb) continue
Continuing.
fibonacci was passed 0.
(gdb) continue
Continuing.
fibonacci was passed 1.
(gdb) continue
Continuing.
Fibonacci(3) is 3.
[Inferior 1 (process 13866) exited normally]
(gdb)
```

### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 2.11 Breakpoint Command Lists
- 《Debugging With GDB》: 5.1.7 Breakpoint Command Lists
