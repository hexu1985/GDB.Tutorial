### 条件断点

最简单的断点会在程序执行到指定位置时中断程序。也可以为断点指定条件。  
条件只是程序语言的一个 Boolean 型的表达式。带条件的断点会在程序执行到底时候计算表达式的值，
只有在条件为真的时候才中断。

- break break-args if (condition)
    设置条件断点的语法为：
    ```
    $ break break-args if (condition)
    ```
    其中`break-args`是可以传递给break以指定断点位置的任何参数。  
    `condition`是布尔表达式。
    
    ```
    $ gcc -g3 -Wall -Wextra -o factorial factorial.c
    $ gdb factorial
    (gdb) list
    6
    7       #include <stdio.h>
    8       #include <stdlib.h>
    9
    10      int factorial(int n) {
    11          int result = 1;
    12          if(n == 0)
    13              return result;
    14          result = factorial(n-1) * n;
    15          return result;
    (gdb) break 11 if n == 4
    Breakpoint 1 at 0x725: file factorial.c, line 11.
    (gdb) info break
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x0000000000000725 in factorial at factorial.c:11
            stop only if n == 4
    (gdb) run 10
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/factorial/factorial 10
    
    Breakpoint 1, factorial (n=4) at factorial.c:11
    11          int result = 1;
    (gdb) print n
    $1 = 4
    (gdb)
    ```

- condition bnum expression 
    可以对正常断点设置条件以将它们转变为条件断点。  
    例如，如果设置了断点3作为无条件断点，但是现在希望添加条件i==3，只要键入：
    ```
    (gdb) cond 3 i == 3
    ```

- condition bnum
    从断点 bnum 里删除条件。这个断点就成为普通的无条件的断点。
    例如，断点3为条件断点，要删除条件，但保持该断点，只要键入：
    ```
    (gdb) cond 3
    ```

    一个condition的例子：
    ```
    $ gcc -g3 -Wall -Wextra -o until-anomaly until-anomaly.c
    $ gdb until-anomaly
    (gdb) list
    1       #include <stdio.h>
    2
    3       int main( void )
    4       {
    5               int i;
    6
    7               for( i = 0;  i < 10;  ++i )
    8                       printf("hello world!");
    9
    10              return 0;
    (gdb) break main
    Breakpoint 1 at 0x652: file until-anomaly.c, line 7.
    (gdb) break 8
    Breakpoint 2 at 0x65b: file until-anomaly.c, line 8.
    (gdb) run
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/until-anomaly/until-anomaly
    
    Breakpoint 1, main () at until-anomaly.c:7
    7               for( i = 0;  i < 10;  ++i )
    (gdb) continue
    Continuing.
    
    Breakpoint 2, main () at until-anomaly.c:8
    8                       printf("hello world!");
    (gdb) display i
    1: i = 0
    (gdb) continue
    Continuing.
    
    Breakpoint 2, main () at until-anomaly.c:8
    8                       printf("hello world!");
    1: i = 1
    (gdb) info break
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x0000555555554652 in main at until-anomaly.c:7
            breakpoint already hit 1 time
    2       breakpoint     keep y   0x000055555555465b in main at until-anomaly.c:8
            breakpoint already hit 2 times
    (gdb) cond 2 i == 5
    (gdb) continue
    Continuing.
    
    Breakpoint 2, main () at until-anomaly.c:8
    8                       printf("hello world!");
    1: i = 5
    (gdb) info break
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x0000555555554652 in main at until-anomaly.c:7
            breakpoint already hit 1 time
    2       breakpoint     keep y   0x000055555555465b in main at until-anomaly.c:8
            stop only if i == 5
            breakpoint already hit 3 times
    (gdb) cond 2
    Breakpoint 2 now unconditional.
    (gdb) info break
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x0000555555554652 in main at until-anomaly.c:7
            breakpoint already hit 1 time
    2       breakpoint     keep y   0x000055555555465b in main at until-anomaly.c:8
            breakpoint already hit 3 times
    (gdb) continue
    Continuing.
    
    Breakpoint 2, main () at until-anomaly.c:8
    8                       printf("hello world!");
    1: i = 6
    (gdb)
    ```

### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 2.10 Conditional Breakpoints
