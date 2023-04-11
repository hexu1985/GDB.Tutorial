### 在GDB中检查变量

在程序里查看数据的常用方式是用 print 命令（缩写为 p），或者用它的同义命令 inspect。
print 命令会计算和打印用程序语言写的表达式的值

- print [/f] expr
    expr 是表达式（用程序语言写的）。缺省情况下，expr 的值会以它的数据类型相近的格式打印；  
    也可以用’/f’选择不同的格式，f 是格式描述符；
    ```
    $ gcc -g3 -Wall -Wextra -o bintree bintree.c
    $ gdb bintree
    (gdb) break 37
    Breakpoint 1 at 0x75a: file bintree.c, line 37.
    (gdb) run
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/bintree/origin/bintree 12 8 5 19 16
    
    Breakpoint 1, insert (btp=0x555555755018 <root>, x=8) at bintree.c:37
    37            if (x < tmp->val) {
    (gdb) p tmp->val
    $1 = 12
    (gdb) p tmp->left
    $2 = (struct node *) 0x0
    (gdb) p tmp->right
    $3 = (struct node *) 0x0
    (gdb) c
    Continuing.
    
    Breakpoint 1, insert (btp=0x555555755018 <root>, x=5) at bintree.c:37
    37            if (x < tmp->val) {
    (gdb) p tmp->val
    $4 = 12
    (gdb) p tmp->left
    $5 = (struct node *) 0x555555756280
    (gdb) p tmp->right
    $6 = (struct node *) 0x0
    (gdb) p *tmp
    $7 = {val = 12, left = 0x555555756280, right = 0x0}
    ```

- print [/f] 
    如果省略了 expr，GDB 将显示最后一次的值（从值的历史记录里；）。  
    这个命令允许用户方便的换一种格式查看相同值。


缺省的，GDB 根据数据类型打印数据值。不过，有时这种方式可能不是你想要的。例如，你可能要以 16 进制打印一个数值，  
或者以 10 进制打印一个指针。或者你想要查看内存中某个地址上的数据，作为字符串或者一个指令。  
要做到这些，在打印值的时候指定输出格式就可以了。

最简单的输出格式的用法是指定如何打印一个已计算过的值。要达到这个目的，在命令print 后加上反斜杠’/'和一个格式符号就可以。

格式符号如下：
- x 将数值作为整型数据，并以 16 进制打印。
- d 打印带符号整型数据
- u 打印以无符号整型数据
- o 以 8 进制打印整形数据
- t 以 2 进制打印整形。’t'代表’two’
- a 打印地址，打印 16 进制的绝对地址和最近符号的偏移量。可以用这个格式找出一个未知地址的位于何处（在哪个函数里）：
- c 将一个整型以字符常量打印。会打印一个数值和它表示的字符。超出 7 位的 ASCII 的数值（大于 127）的字符用 8 进制的数字替代打印。
- f 将数据以浮点类型打印。
- s 如果可能的话，以字符串形式打印。用这个格式，指向单字节数据的指针将以 nll 结尾的字符串打印，单字节数据的数组则会以定长字符串打印。  
    其它的值以它们原本类型打印。

