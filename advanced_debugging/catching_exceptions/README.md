### 捕获异常

用捕获点可以让调试器为某些程序事件中断程序执行，例如 C++异常。
用 catch 命令来设置一个捕获点。
```
catch event
```
在 event 发生时中断。事件可以是下列的：
- throw C++异常的抛出。
- catch C++异常的捕获。

用 info break 命令列出目前的捕获点。

考虑以下示例：

```
#include <iostream>
void f1() {
    throw 42;
}
void f2() {
    throw "pol";
}

int main(int,char**) {
    try {
        f1();
    } catch (int E) {
        std::cout << "caught E=" << E << std::endl;
    }
    f2();
    return 0;
}
```

因为只有在载入了运行库之后，才能使用GDB的catch和throw命令。因此，一定要
在运行程序之后使用这些命令。一种简便的方法是用start命令使程序在main()函数
的第一行停止。

```
$ g++ -g3 -Wall -Wextra -o exception_catcher exception_catcher.cc
$ gdb ./exception_catcher
(gdb) start
Temporary breakpoint 1 at 0xc3e: file exception_catcher.cc, line 17.
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/exception_catcher/exception_catcher

Temporary breakpoint 1, main () at exception_catcher.cc:17
17	        f1();
(gdb) catch throw
Catchpoint 2 (throw)
(gdb) catch catch
Catchpoint 3 (catch)
(gdb) continue
Continuing.

Catchpoint 2 (exception thrown), 0x00007ffff7a6e7be in __cxa_throw () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
(gdb) where
#0  0x00007ffff7a6e7be in __cxa_throw () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#1  0x0000555555554c02 in f1 () at exception_catcher.cc:9
#2  0x0000555555554c43 in main () at exception_catcher.cc:17
(gdb)
```

可以看到，调试器捕获到了C++异常的抛出。

### 参考资料:
- 《The Developer’s Guide to Debugging》:  10.8  Catching Exceptions