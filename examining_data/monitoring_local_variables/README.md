### 在GDB中监视局部变量

在GDB中，可以通过调用info locals命令得到当前栈帧中所有局部变量的值列表。

- info locals 
    打印选定帧上的本地变量，每个一行。选定帧的执行点上可见的所有变量（声明为静态或自动的均可）。

- info args
    打印选定帧的参数，每个参数一行。

一个完整的示例如下：
```
$ gcc -g3 -Wall -Wextra -o swapflaw swapflaw.c
$ gdb swapflaw
(gdb) list
1       /* swapflaw.c: A flawed function that swaps wo integers. */
2       #include <stdio.h>
3       void swap(int a, int b);
4
5       int main( void )
6       {
7               int i = 4;
8               int j = 6;
9
10              printf("i: %d, j: %d\n", i, j);
(gdb) break 10
Breakpoint 1 at 0x660: file swapflaw.c, line 10.
(gdb) break swap
Breakpoint 2 at 0x6b2: file swapflaw.c, line 19.
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/c/swapflaw/swapflaw

Breakpoint 1, main () at swapflaw.c:10
10              printf("i: %d, j: %d\n", i, j);
(gdb) info locals
i = 4
j = 6
(gdb) continue
Continuing.
i: 4, j: 6

Breakpoint 2, swap (a=4, b=6) at swapflaw.c:19
19              int c = a;
(gdb) info locals
c = 0
(gdb) next
20              a = b;
(gdb) info locals
c = 4
(gdb) finish
Run till exit from #0  swap (a=4, b=6) at swapflaw.c:20
main () at swapflaw.c:12
12              printf("i: %d, j: %d\n", i, j);
(gdb) info locals
i = 4
j = 6
(gdb)
```

### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 3.2.6 Monitoring Local Variables
