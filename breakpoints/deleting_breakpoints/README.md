### 在GDB中删除断点

GDB中有两个用来删除断点的命令。delete命令用来基于标识符删除断点。
clear命令使用与创建断点相同的语法删除断点。

- delete breakpoint_list
    删除断点使用数值标识符。断点可以是一个数字，比如delete 2删除第二个断点；
    也可以是数字列表，比如delete 2 4删除第二个和第四个断点。
    ```
    $ gcc -g3 -Wall -Wextra -o swap main.c swapper.c
    $ gdb swap
    (gdb) break main
    Breakpoint 1 at 0x6b2: file main.c, line 5.
    (gdb) break swapper.c:swap
    Breakpoint 2 at 0x73b: file swapper.c, line 3.
    (gdb) info breakpoints
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x00000000000006b2 in main at main.c:5
    2       breakpoint     keep y   0x000000000000073b in swap at swapper.c:3
    (gdb) delete 1
    (gdb) info breakpoints
    Num     Type           Disp Enb Address            What
    2       breakpoint     keep y   0x000000000000073b in swap at swapper.c:3
    (gdb)
    ```

- delete
    删除所有的断点。
    ```
    $ gcc -g3 -Wall -Wextra -o swap main.c swapper.c
    $ gdb swap
    (gdb) break main
    Breakpoint 1 at 0x6b2: file main.c, line 5.
    (gdb) break swapper.c:swap
    Breakpoint 2 at 0x73b: file swapper.c, line 3.
    (gdb) info breakpoints
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x00000000000006b2 in main at main.c:5
    2       breakpoint     keep y   0x000000000000073b in swap at swapper.c:3
    (gdb) delete
    Delete all breakpoints? (y or n) y
    (gdb) info breakpoints
    No breakpoints or watchpoints.
    (gdb)
    ```

- clear 
    在选定的栈帧上的下一个指令上删除所有的断点。只要选择最内层的帧的话，这是一个在程序中断地方删除断点的好方法。
    ```
    $ gcc -g3 -Wall -Wextra -o swap main.c swapper.c
    $ gdb swap
    (gdb) break main
    Breakpoint 1 at 0x6b2: file main.c, line 5.
    (gdb) break swap
    Breakpoint 2 at 0x73b: file swapper.c, line 3.
    (gdb) info breakpoints
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x00000000000006b2 in main at main.c:5
    2       breakpoint     keep y   0x000000000000073b in swap at swapper.c:3
    (gdb) run
    Starting program: /home/hexu/git/C.And.Cpp.Debugging.Tutorial/gdb/code/c/swap/swap
    
    Breakpoint 1, main () at main.c:5
    5       {
    (gdb) continue
    Continuing.
    i: 3, j: 5
    
    Breakpoint 2, swap (a=0x7fffffffdbd0, b=0x7fffffffdbd4) at swapper.c:3
    3               int c = *a;
    (gdb) info breakpoints
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x00005555555546b2 in main at main.c:5
            breakpoint already hit 1 time
    2       breakpoint     keep y   0x000055555555473b in swap at swapper.c:3
            breakpoint already hit 1 time
    (gdb) clear
    Deleted breakpoint 2
    (gdb) info breakpoints
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x00005555555546b2 in main at main.c:5
            breakpoint already hit 1 time
    (gdb)
    ```

- clear location
    在指定的位置删除所有的断点，工作方式与对应的break命令相似。
    + clear function
        ```
        $ gcc -g3 -Wall -Wextra -o swap main.c swapper.c
        $ gdb swap
        (gdb) break main
        Breakpoint 1 at 0x6b2: file main.c, line 5.
        (gdb) break swap
        Breakpoint 2 at 0x73b: file swapper.c, line 3.
        (gdb) info breakpoints
        Num     Type           Disp Enb Address            What
        1       breakpoint     keep y   0x00000000000006b2 in main at main.c:5
        2       breakpoint     keep y   0x000000000000073b in swap at swapper.c:3
        (gdb) clear swap
        Deleted breakpoint 2
        (gdb) info breakpoints
        Num     Type           Disp Enb Address            What
        1       breakpoint     keep y   0x00000000000006b2 in main at main.c:5
        (gdb)
        ```
    + clear filename:function
        ```
        $ gcc -g3 -Wall -Wextra -o swap main.c swapper.c
        $ gdb swap
        (gdb) break main
        Breakpoint 1 at 0x6b2: file main.c, line 5.
        (gdb) break swapper.c:swap
        Breakpoint 2 at 0x73b: file swapper.c, line 3.
        (gdb) info breakpoints
        Num     Type           Disp Enb Address            What
        1       breakpoint     keep y   0x00000000000006b2 in main at main.c:5
        2       breakpoint     keep y   0x000000000000073b in swap at swapper.c:3
        (gdb) clear swapper.c:swap
        已删除的断点 2
        (gdb) info breakpoints
        Num     Type           Disp Enb Address            What
        1       breakpoint     keep y   0x00000000000006b2 in main at main.c:5
        (gdb)
        ```
    + clear line_number
        ```
        $ gcc -g3 -Wall -Wextra -o swap main.c swapper.c
        $ gdb swap
        (gdb) list swap
        1       void swap(int *a, int *b)
        2       {
        3               int c = *a;
        4               *a = *b;
        5               *b = c;
        6       }
        (gdb) break 4
        Breakpoint 1 at 0x744: file swapper.c, line 4.
        (gdb) info breakpoints
        Num     Type           Disp Enb Address            What
        1       breakpoint     keep y   0x0000000000000744 in swap at swapper.c:4
        (gdb) clear 4
        Deleted breakpoint 1
        (gdb) info breakpoints
        No breakpoints or watchpoints.
        (gdb)
        ```
    + clear filename:line_number
        ```
        $ gcc -g3 -Wall -Wextra -o swap main.c swapper.c
        $ gdb swap
        (gdb) break main
        Breakpoint 1 at 0x6b2: file main.c, line 5.
        (gdb) break swapper.c:4
        Breakpoint 2 at 0x744: file swapper.c, line 4.
        (gdb) info breakpoints
        Num     Type           Disp Enb Address            What
        1       breakpoint     keep y   0x00000000000006b2 in main at main.c:5
        2       breakpoint     keep y   0x0000000000000744 in swap at swapper.c:4
        (gdb) clear swapper.c:4
        Deleted breakpoint 2
        (gdb) info breakpoints
        Num     Type           Disp Enb Address            What
        1       breakpoint     keep y   0x00000000000006b2 in main at main.c:5
        (gdb)
        ```

### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 2.7.1 Deleting Breakpoints in GDB
