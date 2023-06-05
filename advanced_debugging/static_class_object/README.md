### 调试静态构造/析构函数

在C++中，如果程序中含有静态类成员或全局实例化的类对象，那么程序在启动时  
会调用该对象的构造函数。此外，在C++中可以使用一种称为静态初始化程序（static initializer）  
的函数或方法来初始化全局对象和静态对象。

下面是一个示例：

```cpp
MyClass otherGlobal;
int MyClass::veryImportantStatic = somefunction();
```

这些函数和构造函数是在main()函数被调用之前的静态初始化期间被调用。类似地，  
静态或全局类对象的析构函数是在main()函数返回之后的程序关闭期间被调用的。


**由静态初始化程序的顺序依赖性引起的bug**

在下面的示例中，有两个带有静态初始化的变量：全局变量otherGlobal和静态变量
veryImportantStatic:

```cpp
  1 /* MyClass.h */
  2 /* Copyright (c) 2008 Thorsten Groetker, Ulrich Holtmann, Holger Keding,
  3 Markus Wloka. All rights reserved. Your use or disclosure of this source code
  4 is subject to the terms and conditions of the end user license agreement (EULA)
  5 that is part of this software package. */
  6
  7 class MyClass {
  8 public:
  9     MyClass() {
 10         int i;
 11         for(i=0;i<10;i++)
 12             a[i]= veryImportantStatic+i;
 13     }
 14
 15     int a[10];
 16     static int veryImportantStatic;
 17 };

  1 /* MyClass.cc */
  2 /* Copyright (c) 2008 Thorsten Groetker, Ulrich Holtmann, Holger Keding,
  3 Markus Wloka. All rights reserved. Your use or disclosure of this source code
  4 is subject to the terms and conditions of the end user license agreement (EULA)
  5 that is part of this software package. */
  6
  7 #include "MyClass.h"
  8
  9 int somefunction()
 10 {  return 42;}
 11
 12 int MyClass::veryImportantStatic = somefunction();

  1 /* static_conflict.cc */
  2 /* Copyright (c) 2008 Thorsten Groetker, Ulrich Holtmann, Holger Keding, 
  3 Markus Wloka. All rights reserved. Your use or disclosure of this source code 
  4 is subject to the terms and conditions of the end user license agreement (EULA)
  5 that is part of this software package. */
  6 
  7 #include <stdio.h>
  8 #include "MyClass.h"
  9 
 10 MyClass otherGlobal;
 11 
 12 int main() {
 13     printf("otherGlobal.a[3]=%d\n", otherGlobal.a[3]);
 14     return 0;
 15 }
```

这两个变量都是在程序启动期间通过函数调用初始化的。一个隐含的要求是veryImportantStatic  
必须在otherGlobal之前被初始化。但是，程序忽略了这项要求，因此初始化顺序有可能发生错误。

初始化顺序除了依赖编译器之外，还依赖于链接器和链接顺序。我们可以以不同的链接顺序  
来看到初始化顺序不同。

```
$ g++ -g3 -Wall -Wextra   -c -o static_conflict.o static_conflict.cc
$ g++ -g3 -Wall -Wextra   -c -o MyClass.o MyClass.cc
$ g++ -o static_conflict1 static_conflict.o MyClass.o
$ g++ -o static_conflict2 MyClass.o static_conflict.o
$ ./static_conflict1
otherGlobal.a[3]=3
$ ./static_conflict2
otherGlobal.a[3]=45
```

从输出结果可以看出，当先链接static_conflict.o时优先初始化static_conflict.cc中的otherGlobal，  
相反当先链接MyClass.o时优先初始化MyClass.cc中的veryImportantStatic。


**识别静态初始化程序的栈跟踪**

构造函数或析构函数的顺序问题可能非常模糊。在搜索这些bug时，最有用的信息就是栈跟踪：

```
$ g++ -g3 -Wall -Wextra   -c -o static_conflict.o static_conflict.cc
$ g++ -g3 -Wall -Wextra   -c -o MyClass.o MyClass.cc
$ g++ -o static_conflict1 static_conflict.o MyClass.o
$ gdb static_conflict1
(gdb) break MyClass
Breakpoint 1 at 0x1212: file MyClass.h, line 9.
(gdb) break somefunction
Breakpoint 2 at 0x1149: file MyClass.cc, line 10.
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/static_conflict/static_conflict2
warning: Error disabling address space randomization: Operation not permitted

Breakpoint 2, somefunction () at MyClass.cc:10
10      {  return 42;}
(gdb) bt
#0  somefunction () at MyClass.cc:10
#1  0x00005562dae8117e in __static_initialization_and_destruction_0 (__initialize_p=1, __priority=65535) at MyClass.cc:12
#2  0x00005562dae8119e in _GLOBAL__sub_I__Z12somefunctionv () at MyClass.cc:12
#3  0x00005562dae8129d in __libc_csu_init ()
#4  0x00007fbf4a12b010 in __libc_start_main (main=0x5562dae811a0 <main()>, argc=1, argv=0x7fffe736de68, init=0x5562dae81250 <__libc_csu_init>, fini=<optimized out>, rtld_fini=<optimized out>,
    stack_end=0x7fffe736de58) at ../csu/libc-start.c:264
#5  0x00005562dae8108e in _start ()
(gdb) continue
Continuing.

Breakpoint 1, MyClass::MyClass (this=0x0) at MyClass.h:9
9           MyClass() {
(gdb) bt
#0  MyClass::MyClass (this=0x0) at MyClass.h:9
#1  0x00005562dae811f5 in __static_initialization_and_destruction_0 (__initialize_p=1, __priority=65535) at static_conflict.cc:10
#2  0x00005562dae8120f in _GLOBAL__sub_I_otherGlobal () at static_conflict.cc:15
#3  0x00005562dae8129d in __libc_csu_init ()
#4  0x00007fbf4a12b010 in __libc_start_main (main=0x5562dae811a0 <main()>, argc=1, argv=0x7fffe736de68, init=0x5562dae81250 <__libc_csu_init>, fini=<optimized out>, rtld_fini=<optimized out>,
    stack_end=0x7fffe736de58) at ../csu/libc-start.c:264
#5  0x00005562dae8108e in _start ()
(gdb)
```

注意，栈跟踪中并没有main()函数，因为它尚未被调用。


**在静态初始化之前连接调试器**

在某些情况下，可能必须将调试器连接到正在运行的进程，然后对初始化例程进行调试。  
由于静态初始化例程是在程序启动阶段被调用的，所以在连接调试器时，它们已经执行。  
因此在初始化阶段应减慢程序的执行速度。添加另外一个静态初始化调用，延迟程序的执行，  
这样就有足够时间连接调试器。

以下代码片段提供了一个示例：

```cpp
#include <stdlib.h>
#include <unistd.h>

static int delay_done=0;
static int ask_mice() {
    while (!delay_done)
        sleep(10);
    return 42;
}

static int pol = ask_mice();
```

连接正在运行的进程，在调试器中设置所有需要的断点。然后再改delay_done变量，使程序继续执行。

```
$ g++ -g3 -Wall -Wextra   -c -o initial_delay.o initial_delay.cc
$ g++ -o static_conflict1 initial_delay.o static_conflict.o MyClass.o
$ g++ -o static_conflict2 initial_delay.o MyClass.o static_conflict.o
$ ./static_conflict1 &
[1] 15543
$ sudo gdb static_conflict1 15543
(gdb) break MyClass
Breakpoint 1 at 0x55c49ec347a8: file MyClass.h, line 11.
(gdb) break somefunction
Breakpoint 2 at 0x55c49ec347db: file MyClass.cc, line 10.
(gdb) bt
#0  0x00007f238031f654 in __GI___nanosleep (requested_time=requested_time@entry=0x7fffaf9bdfa0, remaining=remaining@entry=0x7fffaf9bdfa0) at ../sysdeps/unix/sysv/linux/nanosleep.c:28
#1  0x00007f238031f55a in __sleep (seconds=0) at ../sysdeps/posix/sleep.c:55
#2  0x000055c49ec346f2 in ask_mice () at initial_delay.cc:17
#3  0x000055c49ec3471d in __static_initialization_and_destruction_0 (__initialize_p=1, __priority=65535) at initial_delay.cc:21
#4  0x000055c49ec34739 in _GLOBAL__sub_I_initial_delay.cc(void) () at initial_delay.cc:21
#5  0x000055c49ec3487d in __libc_csu_init ()
#6  0x00007f238025cc18 in __libc_start_main (main=0x55c49ec3473b <main()>, argc=1, argv=0x7fffaf9be138, init=0x55c49ec34830 <__libc_csu_init>, fini=<optimized out>, rtld_fini=<optimized out>,
    stack_end=0x7fffaf9be128) at ../csu/libc-start.c:266
#7  0x000055c49ec345fa in _start ()
(gdb) f 2
#2  0x000055c49ec346f2 in ask_mice () at initial_delay.cc:17
17              sleep(10);
(gdb) list
12      #endif
13
14      static int delay_done=0;
15      static int ask_mice() {
16          while (!delay_done)
17              sleep(10);
18          return 42;
19      }
20
21      static int pol = ask_mice();
(gdb) set var delay_done=1
(gdb) continue
Continuing.

Breakpoint 1, MyClass::MyClass (this=0x55c49ee35040 <otherGlobal>) at MyClass.h:11
11              for(i=0;i<10;i++)
(gdb) c
Continuing.

Breakpoint 2, somefunction () at MyClass.cc:10
10      {  return 42;}
(gdb) c
Continuing.
otherGlobal.a[3]=3
[Inferior 1 (process 15543) exited normally]
(gdb)
```

注意，这种方法还依赖于静态初始化代码的特定执行顺序，即ask_mice()首先被调用。所以要
把initial_delay.o放在链接顺序的最前面。


### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.5 Debugging Static Constructor/Destructor Problems
