### 在GDB中设置监视点

在一个表达式改变时，可以用监视点来中断程序运行，而不需要预先知道表达式在哪里发生变化。（这类断点有时称为数据断点）。
表达式可以简单如单个变量的值，也可以复杂如多个变量用操作符结合起来。例如：
1.单个变量值的引用
2.一个地址转换为一个恰当的数据类型。比如，‘*(int *)0×12345678’会在指定的地址监视一个 4 字节长的区域（认为是一个整形）。
3.任意复杂的表达式，例如‘a*b + c/d’。表达式可以程序语言的任何正确的操作符

- watch expr [thread threadnum]
    设置一个表达式监视点。在表达式 expr 被被改写和值改变时 GDB 会中断程序。最简单（也是最常用的）的用法是监视一个变量：
    ```
    watch var
    ```
    该命令会导致每当var改变值时GDB都会中断。
    如果命令包含[thread threadnum]参数，GDB 只会在 threadnum 标识的线程改变表达式 expr 的值时中断。注意，只在硬件监视点上 GDB 才起作用。
    ```
    $ gcc -g3 -Wall -Wextra -o test2 test2.c 
    $ gdb test2
    (gdb) break main
    Breakpoint 1 at 0x64e: file test2.c, line 6.
    (gdb) run
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/test2/test2
    
    Breakpoint 1, main () at test2.c:6
    6           i = 3;
    (gdb) watch i > 4
    Hardware watchpoint 2: i > 4
    (gdb) continue
    Continuing.
    i is 3.
    
    Hardware watchpoint 2: i > 4
    
    Old value = 0
    New value = 1
    main () at test2.c:10
    10          printf("i is %d.\n", i);
    (gdb)
    ```

- rwatch expr [thread threadnum]
    程序读表达式的值时中断。

- awatch expr [thread threadnum]
    读或写表达式时中断。

- info watchpoints
    打印监视点，断点和捕获点列表；和 info break 相同。

### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 2.12 Watchpoints
- 《Debugging With GDB》: 5.1.2 Setting Watchpoints
