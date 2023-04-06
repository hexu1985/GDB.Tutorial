### 禁用断点

每个断点都可以被启用或禁用。只用当GDB遇到启用的断点时，才会暂停程序的执行；它会忽略禁用的断点。

可以用 enable 和 disable 命令禁用和激活断点，可选择指定一个或多个断点号作为参数。

- disable breakpoint-list
    禁用指定断点，其中`breakpoint-list`是使用空格分隔的列表，其中有一个和多个断点表示符。例如，
    ```
    (gdb) disable 3
    ```
    将禁用第三个断点。

- disable
    不带参数的disable，禁用所有的断点。

- enable breakpoint-list
    启用指定断点，其中`breakpoint-list`是使用空格分隔的列表，其中有一个和多个断点表示符。例如，
    ```
    (gdb) enable 1 5
    ```
    将启用第一个和第五个断点。

- enable
    不带参数的enable，启用所有的断点。

- enable once breakpoint-list
    临时激活指定的断点。GDB 会在这个断点中断程序之后立即禁用它。

断点有四种激活状态中的任意一种：
1. 已激活的（Enabled）。断点会中断程序。这个状态是用 break 命令发起的。
2. 已禁用的（Disabled）。断点不再影响程序。
3. 激活一次（Enabled once）。断点会中断程序，但会变成 disabled 状态。
4. 激活并删除（Enabled for deletion）。断点中断程序，但中断后立即就永久删除。这个状态是用 tbreak 命令发起的。

### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 2.7.2 Disabling Breakpoints in GDB
