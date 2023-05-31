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

