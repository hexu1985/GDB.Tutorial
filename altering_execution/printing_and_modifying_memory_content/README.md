### 输出和修改内存内容

所有调试器都提供了某种用于检查（有时是修改）内存内容的特性。
GDB中的相关命令是whatis、print、x和set var。
内存位置可以用0xbf7a5b4这样的形式表示，也可以通过指向堆内存
的变量表示，例如argv[1]。

这里再次以capitalize程序为例，演示如何读取并修改main()函数的argc和argv变量。

第一步是使用whatis命令确定argv的类型，再用print命令确定它的当前值：

```
$ gcc -g3 -Wall -Wextra -o capitalize capitalize.c
$ gdb capitalize
(gdb) start "foofoo, FooBar and Bar!" foo bar
Temporary breakpoint 1 at 0x8bf: file capitalize.c, line 33.
Starting program: /home/hexu/git/GDB.Tutorial/code/c/capitalize/capitalize "foofoo, FooBar and Bar!" foo bar

Temporary breakpoint 1, main (argc=4, argv=0x7fffffffd248) at capitalize.c:33
33	    int hits_total=0;
(gdb) whatis argv
type = char **
(gdb) print argv
$1 = (char **) 0x7fffffffd248
(gdb)
```

变量argv是一个字符数组，因此可以使用表达式`argv[<n>]`来访问它的个别数组元素：

```
(gdb) whatis argv[0]
type = char *
(gdb) print argv[0]
$2 = 0x7fffffffd6bb "/home/hexu/git/GDB.Tutorial/code/c/capitalize/capitalize"
(gdb) print argv[1]
$3 = 0x7fffffffd6f4 "foofoo, FooBar and Bar!"
(gdb)
```

@操作符后面紧跟参数5可以通知GDB调试器argv是一个数组，并通知GDB显示前5个元素。
注意，数组以一个空指针终止，因此实际上有argc+1个元素：

```
(gdb) print argc
$4 = 4
(gdb) print *argv@5
$5 = {0x7fffffffd6bb "/home/hexu/git/GDB.Tutorial/code/c/capitalize/capitalize", 0x7fffffffd6f4 "foofoo, FooBar and Bar!", 0x7fffffffd70c "foo", 0x7fffffffd710 "bar", 0x0}
(gdb)
```

也可以用x命令直接访问内存，此命令只需要地址和一个可选的格式：

```
(gdb) x/5xg 0x7fffffffd248
0x7fffffffd248:	0x00007fffffffd6bb	0x00007fffffffd6f4
0x7fffffffd258:	0x00007fffffffd70c	0x00007fffffffd710
0x7fffffffd268:	0x0000000000000000
(gdb)
```

x/5xg命令将输出5个大小为8字节的指针。
也可以用x/40c命令输出字符串argv[1]，此命令输出40字节的字符：

```
(gdb) p argv[1]
$7 = 0x7fffffffd6f4 "foofoo, FooBar and Bar!"
(gdb) x/40c 0x7fffffffd6f4
0x7fffffffd6f4:	102 'f'	111 'o'	111 'o'	102 'f'	111 'o'	111 'o'	44 ','	32 ' '
0x7fffffffd6fc:	70 'F'	111 'o'	111 'o'	66 'B'	97 'a'	114 'r'	32 ' '	97 'a'
0x7fffffffd704:	110 'n'	100 'd'	32 ' '	66 'B'	97 'a'	114 'r'	33 '!'	0 '\000'
0x7fffffffd70c:	102 'f'	111 'o'	111 'o'	0 '\000'	98 'b'	97 'a'	114 'r'	0 '\000'
0x7fffffffd714:	67 'C'	76 'L'	85 'U'	84 'T'	84 'T'	69 'E'	82 'R'	95 '_'
(gdb)
```

现在通过改变“Foo”和“Bar”的顺序来为argc和argv变量创建新值，并且在“Foo”和“Bar”之间添加一个新的参数。
先前为argv分配的内存无法容纳argv[5]，因此必须调用malloc重新为argv分配内存，使其可以容纳6个8字节指针。

```
(gdb) set var argc=5
(gdb) print/x malloc(6*8)
$8 = 0x7ffff7fa41a0
(gdb) set var argv=0x7ffff7fa41a0
(gdb)
```

接下来设置argv中的所有指针。复制原来的argv[0]和argv[1]指针，并为其他参数使用字符串字面值。
注意，GDB在堆上分配表达式中使用的字符串字面值，并且不释放它们，因此不必担心内存突然被释放。

```
(gdb) set var argv[0] = 0x7fffffffd6bb
(gdb) set var argv[1] = 0x7fffffffd6f4
(gdb) set var argv[2] = "BAR"
(gdb) set var argv[3] = "and"
(gdb) set var argv[4] = "Foo"
(gdb) set var argv[5] = 0
(gdb) print *argv@6
$9 = {0x7fffffffd6bb "/home/hexu/git/GDB.Tutorial/code/c/capitalize/capitalize", 0x7fffffffd6f4 "foofoo, FooBar and Bar!", 0x7ffff7fa41d0 "BAR", 0x7ffff7fa41e0 "and", 0x7ffff7fa41f0 "Foo", 0x0}
(gdb)
```

最后一步是将字符串“foofoo, FooBar and Bar!”修改为“ABCDEF, FooBar and Bar!”。
我们通过访问`argv[1][<n>]`来重写：

```
(gdb) set var argv[1][0]='A'
(gdb) set var argv[1][1]='B'
(gdb) set var argv[1][2]='C'
(gdb) set var argv[1][3]='D'
(gdb) set var argv[1][4]='E'
(gdb) set var argv[1][5]='F'
(gdb)
```

另外，我们也可以通过调用strncpy来修改字符串内容：`call strncpy(argv[1], "ABCDEF", 6)`

现在完成了内存修改，运行程序，查看结果是否正确：

```
(gdb) cont
Continuing.
Total 4 hits:
original: ABCDEF, FooBar and Bar!
modified: ABCDEF, FOOBAR AND BAR!
[Inferior 1 (process 13688) exited normally]
(gdb)
```

内存的修改并不限于少数几个变量。付出足够的努力和细心，就可以修改全部内存内容。
当然，也有可能会意外地破坏内存。

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.10.6 Printing and Modifying Memory Content