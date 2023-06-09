### 在GDB中修改变量

通过赋值表达式可以改变变量的值。例如：

```
(gdb) print x=4
```

将 4 赋值给变量 x，接着打印这个赋值表达式的值（此值是 4）。

如果对赋值操作的结果不感兴趣的话，使用 set 命令替代 print 命令就可以了。  
set 和 print 一样赋值，只是不打印赋值的结果，不将此值加入到值历史里。  
此表达式只用于其作用效果中。

如果 set 命令的参数字符串的开头和 set 子命令一样的话，使用 set variable 命令，  
而不要只用 set 命令。此命令和 set 命令一样，除了它没有子命令外。

例如，如果程序有个变量叫 width，如果试图用`set width=13`来设置一个新值的的话，  
就会导致一个错误，因为 GDB 有个命令叫`set width`:

```
(gdb) whatis width
type = double
(gdb) p width
$4 = 13
(gdb) set width=47
Invalid syntax in expression.
```

当然，错误在于表达式`=47`。要想设置程序变量 width，用

```
(gdb) set var width=47
```

由于 set 命令有许多子命令，很容易就和程序变量名冲突，用 set variable 命令  
替代 set 命令就能很好的解决这个问题。

下面给出一个完整的例子：

```
$ gcc -g3 -Wall -Wextra -o capitalize capitalize.c
$ gdb capitalize
(gdb) break 23
Breakpoint 1 at 0x128d: file capitalize.c, line 23.
(gdb) run "Foofoo, foobar and Bar!" foo bar
Starting program: /home/hexu/git/GDB.Tutorial/code/c/capitalize/capitalize "Foofoo, foobar and Bar!" foo bar
warning: Error disabling address space randomization: Operation not permitted

Breakpoint 1, capitalize_str (str=0x55b701f502a0 "Foofoo, foobar and Bar!", name=0x7fff2f6d281f "foo") at capitalize.c:23
23              if (strncasecmp(str+n,name,len_name)==0) {
(gdb) print n
$1 = 0
(gdb) c
Continuing.

Breakpoint 1, capitalize_str (str=0x55b701f502a0 "FOOfoo, foobar and Bar!", name=0x7fff2f6d281f "foo") at capitalize.c:23
23              if (strncasecmp(str+n,name,len_name)==0) {
(gdb) print n
$2 = 4
(gdb) print str+n
$3 = 0x55b701f502a4 "oo, foobar and Bar!"
(gdb) set var n=3
(gdb) print str+n
$4 = 0x55b701f502a3 "foo, foobar and Bar!"
(gdb) delete 1
(gdb) continue
Continuing.
Total 5 hits:
original: Foofoo, foobar and Bar!
modified: FOOFOO, FOOBAR and BAR!
[Inferior 1 (process 76) exited normally]
(gdb)
```

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.10.1 Changing a Variable

