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
  3 // example of less-than-well-written class
  4 class STR {
  5 public:
  6     STR(const char* a) {
  7         s=a;
  8         num++;
  9     }
 10     STR(const STR& a) {
 11         s=a.s;
 12         num++;
 13     }
 14     ~STR() {
 15         num--;
 16     }
 17     const char* c_str() const {
 18         return s.c_str();
 19     }
 20     const STR& operator+ (const STR& a) {
 21         s += a.s; return *this;
 22     }
 23     int num_objs() const {
 24         return num;
 25     }
 26     STR operator* (int num_copies) const {
 27         std::string tmp("");
 28         for (int n=0; n<num_copies;n++)
 29             tmp += s;
 30         return STR(tmp.c_str());
 31     }
 32 private:
 33     static int num;  // total #objects of this class
 34     std::string s;
 35 };
 36
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

Breakpoint 1, main (argc=1, argv=0x7fffffffdcf8) at break_str.cc:46
46          show(x);
(gdb) step
STR::STR (this=0x7fffffffdbd0, a=...) at break_str.cc:10
10          STR(const STR& a) {
(gdb) step
11              s=a.s;
(gdb) step
12              num++;
(gdb) step
13          }
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

