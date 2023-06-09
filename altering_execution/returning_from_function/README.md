### 从函数里返回

在GDB中，我们可以从函数退出，跳过剩下的语句，并且可以修函数的返回值。  
这些都通过使用`return [<expr>]`命令来完成。

- return 
- return expression 

用 return 命令可以取消函数调用的执行。如果指定一个 expression 表达式参数，其值将是此函数的返回值。  
要是运用 return，GDB 将销毁当前选定的堆栈帧（包括此栈内的所有帧）。可以认为是从销毁的堆栈帧中永久返回。  
如果希望设定一个返回值，指定 return 的参数。

return 命令弹出选定的堆栈帧，和在此堆栈帧里所有的其他帧，使得其调用函数为最内层的堆栈帧。  
这个调用函数将变成为当前选定的堆栈帧。 定的值会存储于寄存器，用作函数的返回值。  
return 命令不会继续执行程序；它会让程序中断地像函数刚刚返回的状态那样。  
与此相反，finish 命令会继续执行直到此堆栈帧自然返回。


我们先给出一个修改函数的返回值的示例，在capitalize程序里，在第30行函数刚要返回时，  
使用return命令重写capitalize_str的返回值：

```
$ gcc -g3 -Wall -Wextra -o capitalize capitalize.c
$ gdb capitalize
(gdb) set args "Foofoo, foobar and Bar!" foo bar
(gdb) tbreak 30
Temporary breakpoint 1 at 0x12e6: file capitalize.c, line 30.
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/c/capitalize/capitalize "Foofoo, foobar and Bar!" foo bar
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 1, capitalize_str (str=0x55a68dc562a0 "FOOfoo, FOObar and Bar!", name=0x7ffc9b6ce81f "foo") at capitalize.c:30
30      }
(gdb) return 40
Make capitalize_str return now? (y or n) y
#0  0x000055a68cf05358 in main (argc=4, argv=0x7ffc9b6cdf38) at capitalize.c:39
39              hits_total += capitalize_str( mycopy, argv[na] );
(gdb) cont
Continuing.
Total 42 hits:
original: Foofoo, foobar and Bar!
modified: FOOfoo, FOOBAR and BAR!
[Inferior 1 (process 165) exited normally]
(gdb)
```

正常的返回值是4，而程序现在输出42。这说明操纵返回值是有效的。

接下来，我们给出一个中止函数调用的示例，在capitalize程序里，当程序执行到change_word的中间时，  
可以使用此特性从change_word和capitalize_str直接返回到main：

```
$ gcc -g3 -Wall -Wextra -o capitalize capitalize.c
$ gdb capitalize
(gdb) set args "Foofoo, foobar and Bar!" foo bar
(gdb) tbreak 12
Temporary breakpoint 1 at 0x11fc: file capitalize.c, line 12.
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/c/capitalize/capitalize "Foofoo, foobar and Bar!" foo bar
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 1, change_word (str=0x55c5506ef2a0 "Foofoo, foobar and Bar!", len=3) at capitalize.c:12
12          for (i=0; i<len; i++)
(gdb) p str
$1 = 0x55c5506ef2a0 "Foofoo, foobar and Bar!"
(gdb) next
13              str[i]=toupper(str[i]);
(gdb) next
12          for (i=0; i<len; i++)
(gdb) next
13              str[i]=toupper(str[i]);
(gdb) next
12          for (i=0; i<len; i++)
(gdb) p str
$2 = 0x55c5506ef2a0 "FOofoo, foobar and Bar!"
(gdb) up
#1  0x000055c54fe882cd in capitalize_str (str=0x55c5506ef2a0 "FOofoo, foobar and Bar!", name=0x7ffcc946781f "foo") at capitalize.c:24
24                   change_word( str+n, len_name );
(gdb) return 0
Make capitalize_str return now? (y or n) y
#0  0x000055c54fe88358 in main (argc=4, argv=0x7ffcc94663f8) at capitalize.c:39
39              hits_total += capitalize_str( mycopy, argv[na] );
(gdb) cont
Continuing.
Total 2 hits:
original: Foofoo, foobar and Bar!
modified: FOofoo, fooBAR and BAR!
[Inferior 1 (process 171) exited normally]
(gdb)
```

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.10.3 Changing the Return Value of a Function, 10.10.4 Aborting Function Calls

