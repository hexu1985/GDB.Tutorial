### 在C++中设置断点

**在C++函数、方法和操作符中设置断点**

C++支持函数重载，因此可能有多个同名C++函数，这些函数具有不同的参数类型和参数个数。  
为了找到函数，调试器必须知道它的准确签名（signature）。在C++中，函数签名包含名称、  
类名称、所有参数的类型和命名空间。在模板函数特化（template functions specialization）中，  
签名还包括模板参数。

在GDB中，如果C::foo是一个具有重载的成员函数，我们用break C::foo命令设置断点，则会出现  
一个断点可能对应于多个位置的情况，具体的：

```
$ g++ -g3 -Wall -Wextra -o overload overload.cc
$ gdb overload
(gdb) break foo
Breakpoint 1 at 0x71d: foo. (3 locations)
(gdb) info breakpoints
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   <MULTIPLE>
1.1                         y     0x000000000000071d in C::foo(int) at overload.cc:10
1.2                         y     0x0000000000000736 in C::foo() const at overload.cc:11
1.3                         y     0x0000000000000752 in C::foo(char, bool) at overload.cc:12
(gdb)
```

这是因为方法名称foo并未指定参数，所以名称是不明确的，GDB为所有匹配方法设置断点。  
一种解决办法是通过指定完整的函数签名：

```
$ g++ -g3 -Wall -Wextra -o overload overload.cc
$ gdb overload
(gdb) break C::foo(int)
Breakpoint 1 at 0x71d: file overload.cc, line 10.
(gdb) info breakpoints
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x000000000000071d in C::foo(int) at overload.cc:10
(gdb)
```

也可以键入部分签名，然后让GDB来完成其余工作。GDB提供里一种方便的机制，可以自动完成签名，这类似于命令行解释器自动完成文件。  
在函数后面跟Tab键或ESC，即可触发自动完成机制。例如，键入break C::foo，后跟Tab键。

```
$ g++ -g3 -Wall -Wextra -o overload overload.cc
$ gdb overload
(gdb) break C::foo(
C::foo() const      C::foo(char, bool)  C::foo(int)
(gdb)
```

使用GDB的ptype命令可以找到类方法的更多信息。此命令列出类的所有元素（包括方法）：

```
$ g++ -g3 -Wall -Wextra -o overload overload.cc
$ gdb overload
(gdb) ptype C
type = class C {
  private:
    int n;

  public:
    C(int);
    int foo(int);
    int foo(void) const;
    int foo(char, bool);
    C & operator=(const C &);
}
(gdb)
```

GDB命令`info functions <expr>`将查找与`<expr>`匹配的所有 全局函数和成员函数：

```
$ g++ -g3 -Wall -Wextra -o overload overload.cc
$ gdb overload
(gdb) info functions C::foo
All functions matching regular expression "C::foo":

File overload.cc:
int C::foo() const;
int C::foo(char, bool);
int C::foo(int);
(gdb)
```
