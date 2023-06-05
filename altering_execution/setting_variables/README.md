### 在GDB中设置变量

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


### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 3.3 Setting Variables from Within GDB/DDD/Eclipse

