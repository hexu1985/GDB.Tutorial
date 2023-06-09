### 在GDB中调用函数

调试器提供的另一个重要特性是通过调用C或C++函数来操纵程序。  
在GDB中，可以使用call或print命令。

- print expr
    计算表达式 expr 和打印结果。expr 可以包含调用被调试程序里的函数。

- call expr
    计算表达式 expr 而不打印空返回值。  
    call 是 print 命令的变体，使用它可以用来执行无返回值的函数（也称为 void 函数），但不会将 void 返回值打印，GDB 会负责打印。  
    如果结果不是 void，GDB 会打印且存入值历史。  

下面通过多调用一次capitalize_str函数将字符串and转换为大写来演示此特性：

```
$ gcc -g3 -Wall -Wextra -o capitalize capitalize.c
$ gdb capitalize
(gdb) set args "Foofoo, foobar and Bar!" foo bar
(gdb) tbreak 39
Temporary breakpoint 1 at 0x1332: file capitalize.c, line 39.
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/c/capitalize/capitalize "Foofoo, foobar and Bar!" foo bar
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 1, main (argc=4, argv=0x7fff27f8a588) at capitalize.c:39
39              hits_total += capitalize_str( mycopy, argv[na] );
(gdb) call capitalize_str( mycopy, "and" )
$1 = 1
(gdb) cont
Continuing.
Total 4 hits:
original: Foofoo, foobar and Bar!
modified: FOOfoo, FOOBAR AND BAR!
[Inferior 1 (process 114) exited normally]
(gdb)
```

注意，AND现在是大写的了。

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.10.2 Calling Functions
