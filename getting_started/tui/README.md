## GDB图形化界面(TUI)


### 简介
TUI（TextUser Interface）为GDB调试的文本用户界面，可以方便地显示源代码、汇编和寄存器文本窗口，甚至可以显示程序及断点运行状态。


### 窗口介绍及切换

1. 窗口介绍

    该模式下有五种窗口

    - (cmd)command 命令窗口. 可以键入调试命令
    - (src)source 源代码窗口. 显示当前行,断点等信息
    - (asm)assembly 汇编代码窗口
    - (reg)register 寄存器窗口
    - split 源码和汇编混合窗口

2. 窗口切换

    - 使用layout命令打开src/asm/reg/split窗口
    - 使用focus命令切换激活的窗口,可简写为fs
    - 使用TUI快捷键
        + 前置快捷键：ctrl + x 即下列快捷键均需要一起按下ctrl+x后松开，再和下列按键组合
        + 组合键
            ```
            a     //关闭/打开TUI界面，注意：虽然在进入gdb调试后也能通过该命令打开TUI界面，但是会出现一些异常，建议还是进入GDB的时候加入-tui参数
            1     //使TUI只显示一个窗口
            2     //使TUI显示两个窗口，连续使用该快捷键可在三种窗口之间切换（但同时只能显示两个窗口）
            o     //切换激活窗口，效果类似focus
            s     //在TUI模式和TUISingleKey模式之间切换，
            ```

3. 窗口属性

    窗口大小使用winheight调节,单位：行数
    例如
    ```
    winheight src +5
    winheight src -4
    ```

4. 窗口刷新
    `ctrl+l`可以刷新窗口，可以避免调试时屏幕混乱


### TUI模式下指令介绍

当进入该模式，方向键和page up/down都编程了翻阅源码的按键，如何浏览或使用上一条gdb指令呢

- ctrl+p previous 上一条
- ctrl+n next 下一条
- ctrl+b back 命令行光标前移
- ctrl+f forward 命令行光标后移

或者，切换GDB focus的窗口，使其激活cmd窗口，而不是src窗口，这样就和普通模式的GDB操作相同啦


### 断点状态显示

在tui模式下，能够显示程序执行的状态和断点状态

- B表示该断点已执行
- b表示该断点未执行
- +表示断点使能
- -表示断点失能


### 参考资料:

GDB调试之图形化界面（TUI）: <https://blog.csdn.net/bsp_mpu6050/article/details/107886292>
