### 在GDB中调用程序函数

- print expr
    计算表达式 expr 和打印结果。expr 可以包含调用被调试程序里的函数。

- call expr
    计算表达式 expr 而不打印空返回值。  
    call 是 print 命令的变体，使用它可以用来执行无返回值的函数（也称为 void 函数），但不会将 void 返回值打印，GDB 会负责打印。  
    如果结果不是 void，GDB 会打印且存入值历史。  
    例如：
    ```
    $ gcc -g3 -Wall -Wextra -o bintree bintree.c
    $ gdb bintree
    (gdb) break 37
    Breakpoint 1 at 0x75a: file bintree.c, line 37.
    (gdb) commands 1
    Type commands for breakpoint(s) 1, one per line.
    End with a line saying just "end".
    >printf "*********** current tree ***********\n"
    >call printtree(root)
    >end
    (gdb) set args 12 8 5 19 16
    (gdb) run
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/bintree/origin/bintree 12 8 5 19 16

    Breakpoint 1, insert (btp=0x555555755018 <root>, x=8) at bintree.c:37
    37            if (x < tmp->val) {
    *********** current tree ***********
    12
    (gdb) continue
    Continuing.
    
    Breakpoint 1, insert (btp=0x555555755018 <root>, x=5) at bintree.c:37
    37            if (x < tmp->val) {
    *********** current tree ***********
    8
    12
    (gdb)
    ```

### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 3.2.1 Inspection in GDB
