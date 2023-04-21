### 禁用断点

每个断点都可以被启用或禁用。只用当GDB遇到启用的断点时，才会暂停程序的执行；它会忽略禁用的断点。

可以用 enable 和 disable 命令禁用和激活断点，可选择指定一个或多个断点号作为参数。

- disable breakpoint-list
    禁用指定断点，其中`breakpoint-list`是使用空格分隔的列表，其中有一个和多个断点表示符。例如，
    ```
    (gdb) disable 3
    ```
    将禁用第三个断点。

- disable
    不带参数的disable，禁用所有的断点。

- enable breakpoint-list
    启用指定断点，其中`breakpoint-list`是使用空格分隔的列表，其中有一个和多个断点表示符。例如，
    ```
    (gdb) enable 1 5
    ```
    将启用第一个和第五个断点。

- enable
    不带参数的enable，启用所有的断点。

- enable once breakpoint-list
    临时激活指定的断点。GDB 会在这个断点中断程序之后立即禁用它。
    ```
    $ gcc -g3 -Wall -Wextra -o test4 test4.c
    $ gdb test4
    (gdb) b test_fun_x
    Breakpoint 1 at 0x63e: file test4.c, line 6.
    (gdb) disable 1
    (gdb) enable once 1
    (gdb) r
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/test4/test4
    execute test_fun_x
    
    Breakpoint 1, test_fun_x () at test4.c:6
    6               printf("test fun x\n");
    (gdb) c
    Continuing.
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    [Inferior 1 (process 14488) exited normally]
    (gdb)
    ```

- enable delete breakpoint-list
    激活断点中断一次，然后删除之。GDB 在程序中断后立即删除这类断点。这类断点用tbreak 命令发起。
    ```
    $ gcc -g3 -Wall -Wextra -o test4 test4.c
    $ gdb test4
    (gdb) b test_fun_x
    Breakpoint 1 at 0x63e: file test4.c, line 6.
    (gdb) disable 1
    (gdb) enable delete 1
    (gdb) b test4.c:17
    Breakpoint 2 at 0x67e: file test4.c, line 17.
    (gdb) info b
    Num     Type           Disp Enb Address            What
    1       breakpoint     del  y   0x000000000000063e in test_fun_x at test4.c:6
    2       breakpoint     keep y   0x000000000000067e in main at test4.c:17
    (gdb) r
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/test4/test4
    execute test_fun_x
    
    Temporary breakpoint 1, test_fun_x () at test4.c:6
    6               printf("test fun x\n");
    (gdb) c
    Continuing.
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    
    Breakpoint 2, main () at test4.c:17
    17          return 0;
    (gdb)
    ```

- enable count N breakpoint-list
    启动断点后可以命中N次，但是命中N此后，该断点就会被自动禁用，不会再次命中。
    ```
    $ gcc -g3 -Wall -Wextra -o test4 test4.c
    $ gdb test4
    (gdb) b test4.c:17
    Breakpoint 1 at 0x67e: file test4.c, line 17.
    (gdb) b test_fun_x
    Breakpoint 2 at 0x63e: file test4.c, line 6.
    (gdb) disable 2
    (gdb) enable count 5 2
    (gdb) info b
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x000000000000067e in main at test4.c:17
    2       breakpoint     dis  y   0x000000000000063e in test_fun_x at test4.c:6
            disable after next 5 hits
    (gdb) r
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/test4/test4
    execute test_fun_x
    
    Breakpoint 2, test_fun_x () at test4.c:6
    6               printf("test fun x\n");
    (gdb) c
    Continuing.
    execute test_fun_x
    
    Breakpoint 2, test_fun_x () at test4.c:6
    6               printf("test fun x\n");
    (gdb) c
    Continuing.
    test fun x
    execute test_fun_x
    
    Breakpoint 2, test_fun_x () at test4.c:6
    6               printf("test fun x\n");
    (gdb) c
    Continuing.
    test fun x
    execute test_fun_x
    
    Breakpoint 2, test_fun_x () at test4.c:6
    6               printf("test fun x\n");
    (gdb) c
    Continuing.
    test fun x
    execute test_fun_x
    
    Breakpoint 2, test_fun_x () at test4.c:6
    6               printf("test fun x\n");
    (gdb) c
    Continuing.
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    
    Breakpoint 1, main () at test4.c:17
    17          return 0;
    (gdb)
    ```

- ignore breakpoint-number N
    与条件断点类似，即在设置断点时可以指定接下来的N次命中都忽略，直到第N+1次命中时运行才暂停。
    ```
    $ gcc -g3 -Wall -Wextra -o test4 test4.c
    $ gdb test4
    (gdb) b test_fun_x
    Breakpoint 1 at 0x63e: file test4.c, line 6.
    (gdb) ignore 1 7
    Will ignore next 7 crossings of breakpoint 1.
    (gdb) r
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/test4/test4
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    test fun x
    execute test_fun_x
    
    Breakpoint 1, test_fun_x () at test4.c:6
    6               printf("test fun x\n");
    (gdb) up
    #1  0x0000555555554674 in main () at test4.c:14
    14                      test_fun_x();
    (gdb) p i
    $1 = 7
    (gdb)
    ```
    

断点有四种激活状态中的任意一种：
1. 已激活的（Enabled）。断点会中断程序。这个状态是用 break 命令发起的。
2. 已禁用的（Disabled）。断点不再影响程序。
3. 激活一次（Enabled once）。断点会中断程序，但会变成 disabled 状态。
4. 激活并删除（Enabled for deletion）。断点中断程序，但中断后立即就永久删除。这个状态是用 tbreak 命令发起的。

### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 2.7.2 Disabling Breakpoints in GDB
