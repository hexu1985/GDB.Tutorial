### 进入C++方法

在C++中，一条简单的C++代码语句可能包含很多隐式或“隐藏的”函数调用。例如，语句`A=B;`  
可能包含对赋值操作符或类型转换操作符的调用。利用类构造函数、操作符、隐式转换等功能，  
可以用非常紧凑的源代码来表示复杂操作。这可以实现很好的抽象和很高的代码密度，而且可以  
用类库来扩展语言。紧凑的C++源代码的一个缺点是不便于调试，如果隐式函数未按照我们的预期  
工作，则必须使这些函数可视化。

考虑以下示例，我们有意编写了一个不完善的字符串类STR：

```
  1 #include <string>
  2 #include <iostream>
  3
  4 // example of less-than-well-written class
  5 class STR {
  6 public:
  7     STR(const char* a) {
  8         s=a;
  9         num++;
 10     }
 11     STR(const STR& a) {
 12         s=a.s;
 13         num++;
 14     }
 15     ~STR() {
 16         num--;
 17     }
 18     const char* c_str() const {
 19         return s.c_str();
 20     }
 21     const STR& operator+ (const STR& a) {
 22         s += a.s; return *this;
 23     }
 24     int num_objs() const {
 25         return num;
 26     }
 27     STR operator* (int num_copies) const {
 28         std::string tmp("");
 29         for (int n=0; n<num_copies;n++)
 30             tmp += s;
 31         return STR(tmp.c_str());
 32     }
 33 private:
 34     static int num;  // total #objects of this class
 35     std::string s;
 36 };
 37 int STR::num=0;
 38
 39 void show( STR z )
 40 {
 41     std::cout << z.num_objs() << ": " << z.c_str() << std::endl;
 42 }
 43
 44 int main(int argc, char* argv[]) {
 45     STR x="abc";
 46     show(x);
 47     show((x+"def") * 3);
 48     return 0;
 49 }
```

**用step-into命令进入到隐式函数中**

看一下第46行的show(X)语句。看起来函数main()是直接调用函数show()，但它对类STR的构造函数有一个  
隐藏的调用。调用此构造函数的原因在于函数show()使用了STR参数，而不是使用STR对象的引用，因此编译器  
在调用函数之前必须创建参数的一个副本。在调试器前进到第46行，并进入到函数调用中，STR构造函数被调用了。  
然后继续使用`step-into`命令来前进，直到达到函数show()。

```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb break_str
(gdb) break 46
Breakpoint 1 at 0xf42: file break_str.cc, line 46.
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str/break_str

Breakpoint 1, main (argc=1, argv=0x7fffffffdcd8) at break_str.cc:46
46          show(x);
(gdb) step
STR::STR (this=0x7fffffffdbb0, a=...) at break_str.cc:11
11          STR(const STR& a) {
(gdb) step
12              s=a.s;
(gdb) step
13              num++;
(gdb) step
14          }
(gdb) step
show (z=...) at break_str.cc:41
41          std::cout << z.num_objs() << ": " << z.c_str() << std::endl;
(gdb)
```

`step-into`命令可以进入任何隐式函数调用中，因此它是查找隐式函数的好方法。当要理解的重要一点是，  
`step-into`命令通常只会在带有调试信息的地方停止。这意味着不带调试信息的隐式函数不容易找到。

注意，`step-into`命令有时会进入C++编译器提供的函数或方法，或者进入程序所包含的各种库中。  
在上例中，当步进到类`std::basic string<...>`的构造函数中时，就发生了这种情况。

**用step-out命令跳过隐式函数**

下面对示例第47行的`show(x + "def")`函数调用进行调试。第47行实际时一个隐式函数调用序列，它等价于  
（但并不一定完全等于）标记为47.1 ... 47.7的伪代码语句。

```
47 show((x+"def") * 3);
  47.1 tmp1=STR::STR("def");
  47.2 tmp2=STR::operator+(x,tmp1);
  47.3 tmp3=operator*(tmp2,3);
  47.4 show(tmp3);
  47.5 STR::˜STR(tmp3);
  47.6 STR::˜STR(tmp2);
  47.7 STR::˜STR(tmp1);
48 return 0;
```

通过反复执行`step-into`命令，可以到达show()的函数体，但这将依次经过函数体的语句47.1至47.3，包括  
这些行中执行的任何函数调用。这样就需要执行大量的`step-into`命令，因此很容易迷失在低层实现细节中。  
在GDB中可以使用finish命令，快捷地离开当前函数。这样，我们就可以更快地从第47行进入第41行地show()函数。

具体细节如下：

```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb break_str
(gdb) break 47
Breakpoint 1 at 0xf6d: file break_str.cc, line 47.
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str/break_str
2: abc

Breakpoint 1, main (argc=1, argv=0x7fffffffdcd8) at break_str.cc:47
47          show((x+"def") * 3);
(gdb) step
STR::STR (this=0x7fffffffdb90, a=0x55555555537d "def") at break_str.cc:7
7           STR(const char* a) {
(gdb) finish
Run till exit from #0  STR::STR (this=0x7fffffffdb90, a=0x55555555537d "def") at break_str.cc:7
0x0000555555554f80 in main (argc=1, argv=0x7fffffffdcd8) at break_str.cc:47
47          show((x+"def") * 3);
(gdb) step
STR::operator+ (this=0x7fffffffdb70, a=...) at break_str.cc:22
22              s += a.s; return *this;
(gdb) finish
Run till exit from #0  STR::operator+ (this=0x7fffffffdb70, a=...) at break_str.cc:22
0x0000555555554f93 in main (argc=1, argv=0x7fffffffdcd8) at break_str.cc:47
47          show((x+"def") * 3);
Value returned is $1 = (const STR &) @0x7fffffffdb70: {static num = 2, s = "abcdef"}
(gdb) step
STR::operator* (this=0x7fffffffdb70, num_copies=3) at break_str.cc:27
27          STR operator* (int num_copies) const {
(gdb) finish
Run till exit from #0  STR::operator* (this=0x7fffffffdb70, num_copies=3) at break_str.cc:27
0x0000555555554faa in main (argc=1, argv=0x7fffffffdcd8) at break_str.cc:47
47          show((x+"def") * 3);
Value returned is $2 = {static num = 3, s = "abcdefabcdefabcdef"}
(gdb) step
show (z=...) at break_str.cc:41
41          std::cout << z.num_objs() << ": " << z.c_str() << std::endl;
(gdb)
```

从第47行开始，也就是从假设的伪代码行47.1开始。`step-into`命令进入到第7行的STR构造函数中。  
`step-out`命令离开构造函数，并在第47.2行停止。注意，GDB报告地位置是`0x<address> ... at main.cpp:47`  
所有伪代码行47.1至47.4都属于同一个源代码行47，但它们具有不同的程序地址，所以可以区分它们。  
执行里一系列`step-into`、`step-out`、`step-into`......命令后，最终就会进入到函数show()中。


**利用临时断点跳过隐式函数**

在有些情况下，我们可能希望从第47行直接进入到函数show()中，而跳过所有计算函数参数的隐式函数调用。  
者可以通过设置临时断点来实现，方法是使用GDB命令tbreak：

```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb break_str
(gdb) break 47
Breakpoint 1 at 0xf6d: file break_str.cc, line 47.
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str/break_str
2: abc

Breakpoint 1, main (argc=1, argv=0x7fffffffdcd8) at break_str.cc:47
47          show((x+"def") * 3);
(gdb) tbreak show
Temporary breakpoint 2 at 0x555555554ea7: file break_str.cc, line 41.
(gdb) cont
Continuing.

Temporary breakpoint 2, show (z=...) at break_str.cc:41
41          std::cout << z.num_objs() << ": " << z.c_str() << std::endl;
(gdb)
```

**从隐式函数调用返回**

如果在嵌套的隐式函数调用中失去线索，也可以使用临时断点。再次从第47行开始，  
反复使用`step-into`命令，直到到达第7行中的第二个构造函数调用。假设现在失去了线索，  
因此只想返回第47行，以便再次使用`step-into`命令进入隐式函数。这时可以结合使用  
栈导航命令up和tbreak前进到第47行的下一条伪代码语句，具体方法如下：

```
$ g++ -g3 -Wall -Wextra -o break_str break_str.cc
$ gdb break_str
(gdb) break 47
Breakpoint 1 at 0xf6d: file break_str.cc, line 47.
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/break_str/break_str
2: abc

Breakpoint 1, main (argc=1, argv=0x7fffffffdcd8) at break_str.cc:47
47          show((x+"def") * 3);
(gdb) step
STR::STR (this=0x7fffffffdb90, a=0x55555555537d "def") at break_str.cc:7
7           STR(const char* a) {
(gdb) step (repeatedly until you reach line 7 again)
...
(gdb) step
31              return STR(tmp.c_str());
(gdb) step
STR::STR (this=0x7fffffffdbb0, a=0x555555769280 "abcdefabcdefabcdef") at break_str.cc:7
7           STR(const char* a) {
(gdb) backtrace
#0  STR::STR (this=0x7fffffffdbb0, a=0x555555769280 "abcdefabcdefabcdef") at break_str.cc:7
#1  0x0000555555555286 in STR::operator* (this=0x7fffffffdb70, num_copies=3) at break_str.cc:31
#2  0x0000555555554faa in main (argc=1, argv=0x7fffffffdcd8) at break_str.cc:47
(gdb) up
#1  0x0000555555555286 in STR::operator* (this=0x7fffffffdb70, num_copies=3) at break_str.cc:31
31              return STR(tmp.c_str());
(gdb) up
#2  0x0000555555554faa in main (argc=1, argv=0x7fffffffdcd8) at break_str.cc:47
47          show((x+"def") * 3);
(gdb) tbreak
Temporary breakpoint 2 at 0x555555554faa: file break_str.cc, line 47.
(gdb) cont
Continuing.

Temporary breakpoint 2, 0x0000555555554faa in main (argc=1, argv=0x7fffffffdcd8) at break_str.cc:47
47          show((x+"def") * 3);
(gdb) step
show (z=...) at break_str.cc:41
41          std::cout << z.num_objs() << ": " << z.c_str() << std::endl;
(gdb)
```

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.3 Stepping in C++ Methods
