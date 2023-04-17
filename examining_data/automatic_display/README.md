### 在GDB中自动显示

如果你觉得需要频繁打印一个表达式的值（来查看其如何改变的），可能要把它加到自动显示列表里  
让 GDB 在每次程序中断时打印这个表达式的值。每个加入列表的表达式会有一个编号来标识；  
要将一个表达式从列表里删除，可以用这个编号。

- display [/f] expr
    将表达式 expr 加入表达式列表，每次程序中断时自动显示。
    例如:
    ```
    $ gcc -g3 -Wall -Wextra -o bintree bintree.c
    $ gdb bintree
    (gdb) break 37
    Breakpoint 1 at 0x75a: file bintree.c, line 37.
    (gdb) set args 12 8 5 19 16
    (gdb) run
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/bintree/origin/bintree 12 8 5 19 16
    
    Breakpoint 1, insert (btp=0x555555755018 <root>, x=8) at bintree.c:37
    37            if (x < tmp->val) {
    (gdb) display *tmp
    1: *tmp = {val = 12, left = 0x0, right = 0x0}
    (gdb) c
    Continuing.
    
    Breakpoint 1, insert (btp=0x555555755018 <root>, x=5) at bintree.c:37
    37            if (x < tmp->val) {
    1: *tmp = {val = 12, left = 0x555555756280, right = 0x0}
    (gdb) c
    Continuing.
    
    Breakpoint 1, insert (btp=0x555555755018 <root>, x=5) at bintree.c:37
    37            if (x < tmp->val) {
    1: *tmp = {val = 8, left = 0x0, right = 0x0}
    (gdb)
    ```

- info display
    打印此前设置的自动显示列表里的表达式，每个表达式带一个编号，但不显示其值。  
    包括禁用的表达式，这类表达式会标明为禁用。也包括目前不能显示的表达式，  
    这类表达式引用了当前不可用的自动变量。
    ```
    $ gcc -g3 -Wall -Wextra -o bintree bintree.c
    $ gdb bintree
    (gdb) break 37
    Breakpoint 1 at 0x75a: file bintree.c, line 37.
    (gdb) set args 12 8 5 19 16
    (gdb) run
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/bintree/origin/bintree 12 8 5 19 16
    
    Breakpoint 1, insert (btp=0x555555755018 <root>, x=8) at bintree.c:37
    37            if (x < tmp->val) {
    (gdb) display *tmp
    1: *tmp = {val = 12, left = 0x0, right = 0x0}
    (gdb) c
    Continuing.
    
    Breakpoint 1, insert (btp=0x555555755018 <root>, x=5) at bintree.c:37
    37            if (x < tmp->val) {
    1: *tmp = {val = 12, left = 0x555555756280, right = 0x0}
    (gdb) info display
    Auto-display expressions now in effect:
    Num Enb Expression
    1:   y  *tmp
    ```

- undisplay dnums...
    从显示列表中删除编号为 dnums 的显示项。也可以用delete display命令替代undisplay。

- disable display dnums...
    禁用编号为 dnums 的显示项。禁用的显示项不会自动输出，但系统仍会记录它。可以再次激活。

- enable display dnums...
    激活编号为 dnums 的显示项。会再次自动显示其表达式的值，直到你禁用它。

### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 3.2.1 Inspection in GDB, 3.2.8 Advanced Options for Print and Display
