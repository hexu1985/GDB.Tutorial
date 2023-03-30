### 在GDB中设置断点

GDB中有许多指定断点的方式，下面是一些最常见的方法。

- break function
    在函数function()的入口（第一行可执行代码）处设置断点。

    ```
    $ gcc -Wall -g3 -Wall -Wextra -o test1 test1.c
    $ gdb test1
    (gdb) break main
    Breakpoint 1 at 0x5fe: file test1.c, line 4.
    ```

- break line_number
    在当前活动源代码文件的line_numnber处设置断点。 对于多文件的项目，这要么是上次使用list命令查看其内容的文件，要么是包含main()的文件。

    ```
    $ gcc -Wall -g3 -Wall -Wextra -o swap main.c swapper.c
    $ gdb swap
    (gdb) break 6
    Breakpoint 1 at 0x6c1: file main.c, line 6.
    ```

    list后再break的效果。

    ```
    $ gcc -Wall -g3 -Wall -Wextra -o swap main.c swapper.c
    $ gdb swap
    (gdb) list swap
    1       void swap(int *a, int *b)
    2       {
    3               int c = *a;
    4               *a = *b;
    5               *b = c;
    6       }
    (gdb) break 6
    Breakpoint 1 at 0x759: file swapper.c, line 6.
    ```

- break filename:line_number
    在源代码文件filename的line_number处设置断点。如果filename不在当前工作目录中，
    则可以给出相对路径名或者完全路径名来帮助GDB查找该文件。

    ```
    $ gcc -Wall -g3 -Wall -Wextra -o swap main.c swapper.c
    $ gdb swap
    (gdb) break swapper.c:1
    Breakpoint 1 at 0x73b: file swapper.c, line 1.
    ```

- break filename:function
    在文件filename中的函数function()的入口处设置断点。重载函数或者使用同名静态函数的程序可能需要使用这种形式。

    ```
    $ gcc -Wall -g3 -Wall -Wextra -o swap main.c swapper.c
    $ gdb swap
    (gdb) break swapper.c:swap
    Breakpoint 1 at 0x73b: file swapper.c, line 3.
    ```

- break
    在不带参数的情况下，break 命令在当前栈里的下一条指令里设置断点

    ```
    $ gcc -Wall -g3 -Wall -Wextra -o factorial factorial.c
    $ gdb factorial
    (gdb) break main
    Breakpoint 1 at 0x763: file factorial.c, line 20.
    (gdb) run 10
    Starting program: /home/hexu/git/C.And.Cpp.Debugging.Tutorial/gdb/code/c/factorial/factorial 10
    
    Breakpoint 1, main (argc=2, argv=0x7fffffffdd88) at factorial.c:20
    20	    if(argc != 2) {
    (gdb) next
    24	    n = atoi(argv[1]);
    (gdb) next
    25	    result = factorial(n);
    (gdb) step
    factorial (n=10) at factorial.c:11
    11	    int result = 1;
    (gdb) next
    12	    if(n == 0) 
    (gdb) break
    Breakpoint 2 at 0x55555555472c: file factorial.c, line 12.
    (gdb) continue
    Continuing.
    
    Breakpoint 2, factorial (n=9) at factorial.c:12
    12	    if(n == 0) 
    ```

- break ... if cond
    带参数设置断点；在每次断点到达时计算 cond 表达式，并且当且仅当表达式的值不为零的时候中断—就是说，如果 cond 表达式为真。
    ‘...’代表可能的指定中断位置的参数（上面描述过的）

    ```
    $ gcc -Wall -g3 -Wall -Wextra -o factorial factorial.c
    $ gdb factorial
    (gdb) break 25
    Breakpoint 1 at 0x7a6: file factorial.c, line 25.
    (gdb) run 10
    Starting program: /home/hexu/git/C.And.Cpp.Debugging.Tutorial/gdb/code/c/factorial/factorial 10
    
    Breakpoint 1, main (argc=2, argv=0x7fffffffdd88) at factorial.c:25
    25	    result = factorial(n);
    (gdb) step
    factorial (n=10) at factorial.c:11
    11	    int result = 1;
    (gdb) break if n == 4
    Breakpoint 2 at 0x555555554725: file factorial.c, line 11.
    (gdb) continue
    Continuing.
    
    Breakpoint 2, factorial (n=4) at factorial.c:11
    11	    int result = 1;
    ```

- tbreak args
    设置一个只中断一次的断点。args 和 break 命令里的参数一样，断点设置也一样，但断点在第一次程序中断后自动删除。

    ```
    $ gcc -Wall -g3 -Wall -Wextra -o factorial factorial.c
    $ gdb factorial
    (gdb) tbreak factorial
    Temporary breakpoint 1 at 0x725: file factorial.c, line 11.
    (gdb) info break
    Num     Type           Disp Enb Address            What
    1       breakpoint     del  y   0x0000000000000725 in factorial at factorial.c:11
    (gdb) run 10
    Starting program: /home/hexu/git/C.And.Cpp.Debugging.Tutorial/gdb/code/c/factorial/factorial 10
    
    Temporary breakpoint 1, factorial (n=10) at factorial.c:11
    11          int result = 1;
    (gdb) info break
    No breakpoints or watchpoints.
    (gdb) continue
    Continuing.
    factorial 10!=3628800
    [Inferior 1 (process 17208) exited normally]
    ```

- rbreak regex
    在所有匹配正则表达式 regex 的函数上设置断点。这个命令会在所有匹配的函数上设置无条件的断点，也打印设置的断点列表。
    一旦这些断点被设置上，它们就和用 break 命令设置的一样了。你可以删除，禁用它们，或者可以和别的断点一样为他们设置条件。
    正则表达式的语法是标准的，就如’grep’工具用的一样。注意，和 shells 用的不一样，例如 foo*匹配开头是 fo，接下来有 0 或者多个 o 的函数。
    在你的正则表达式的开头和结尾有个隐含的.*,所以要想只匹配 foo 开头的函数，用^foo.
    在调试 C++程序，在非特定类的成员函数的重载函数的设置断点上，rbreak 很有用。
    可以用 rbreak 命令在一个程序里的所有函数上设置断点，例如： `(gdb) rbreak .`

    ```
    $ g++ -g3 -Wall -Wextra -o chapter_3 main.cpp student.cpp teacher.cpp
    $ gdb chapter_3
    (gdb) rbreak .*Name
    Breakpoint 1 at 0xcee: file student.cpp, line 12.
    const char *Student::Name();
    Breakpoint 2 at 0xd94: file teacher.cpp, line 10.
    const char *Teacher::Name();
    ```


### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 2.4.1 Setting Breakpoints in GDB
- 《Debugging With GDB》:5.1.1 Setting Breakpoints
