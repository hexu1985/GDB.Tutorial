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
  9     MyClass() {int i; for(i=0;i<10;i++) a[i]= veryImportantStatic+i;}
 10     int a[10];
 11     static int veryImportantStatic;
 12 };

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

