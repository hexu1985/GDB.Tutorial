### 在GDB中检查内存

可以用命令 x（表示”examine”）以多种格式查看内存，而和程序数据类型无关。
- x[/nfu] addr
    用 x 命令查看内存。
    n，f 和 u 都是可选的参数，指定打印多长的内存数据和以何种格式打印之；  
    addr 是需要打印的内存的地址表达式。如果用默认的 nfu，不需要输入反斜杠’/'。

    有几个命令可以方便的设置 addr 的默认值。

    + n，重复次数
        是10 进制正整数(默认是 1)，表示需要显示的内存单元的个数，  
        即从当前地址向后显示n个内存单元的内容，  
        一个内存单元的大小由第三个参数u定义。
    + f，显示格式
        显示格式和 print 命令的格式一样（'x','d','u','o','t','a','c','f','s'），  
        外加'i'（表示机器指令格式）。默认是’x'（16进制）。  
        默认格式在用 x 或 print 命令的时候都会改变。
    + u，单元大小
        单元大小如下：
        - b 字节
        - h 2 节节
        - w 4 字节。默认值。
        - g 8 字节。

    + addr，要打印的起始位置
        addr 是要 GDB 开始打印的内存起始位置。表达式不需要指针值（虽然其可能是一个指针值）；  
        这个表达式总会翻译为内存中一个字节的整型地址。

下面给出完整的示例：
```
$ gcc -g3 -Wall -Wextra -o byteorder byteorder.c
$ gdb byteorder
(gdb) list
1       #include <stdio.h>
2       #include <stdlib.h>
3
4       int main()
5       {
6           union {
7               short  s;
8               char   c[sizeof(short)];
9           } un;
10
(gdb) list
11          un.s = 0x0102;
12          if (sizeof(short) == 2) {
13              if (un.c[0] == 1 && un.c[1] == 2)
14                  printf("big-endian\n");
15              else if (un.c[0] == 2 && un.c[1] == 1)
16                  printf("little-endian\n");
17              else
18                  printf("unknown\n");
19          } else
20              printf("sizeof(short) = %d\n", (int) sizeof(short));
(gdb) b 12
Breakpoint 1 at 0x6a7: file byteorder.c, line 12.
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/c/byteorder/byteorder

Breakpoint 1, main () at byteorder.c:13
13              if (un.c[0] == 1 && un.c[1] == 2)
(gdb) p un.s
$1 = 258
(gdb) p/x un.s
$2 = 0x102
(gdb) x &un.s
0x7fffffffdc06: 0x6d000102
(gdb) x/2xc &un.s
0x7fffffffdc06: 2 '\002'        1 '\001'
(gdb)
```

### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 3.2.7 Examining Memory Directly
