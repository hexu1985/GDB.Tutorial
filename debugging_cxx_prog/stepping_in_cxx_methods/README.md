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

