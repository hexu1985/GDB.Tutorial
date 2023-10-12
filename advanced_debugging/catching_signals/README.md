### 捕捉信号

信号是程序里发生的异步事件。操作系统定义了信号种类，并命名这些信号，给这些信号编号。  
例如，在 Unix 里 SIGINT 是程序在你输入一个中断字符（通常是 Ctrl-c）的时候得到的信号;  
SIGSEGV 是程序在程序引用了离当前所有用到的内存区域都很远的内存时得到的信号；
SIGALRM 在定时器超时的时候发生（只有在程序定义了定时器的时候才发生）。  
某些信号，包括 SIGALRM，是程序的普通组成部分。其它的，像 SIGSEGV，指示发生了错误；
这些信号是致命的（它们会立即结束你的程序），如果你没有预先定义好信号处理函数的话。
SIGINT 不表示程序发生错误，但常常也是致命的，所以可以用来执行中断的目的：杀死进程。

GDB 能够察觉程序里出现的任何信号。可以预先设定 GDB 如何应对每一种信号。
通常，GDB 会将非错误性的信号悄悄的传进程序（因此不会干涉它们在程序里的功能角色），
但在一个错误信号发生时立即中断程序。可以用 handle 命令来改变这些设置。
还有一个命令可以通知调试器生成一个信号，并将它传送给程序。在GDB中，这个命令就是signal。

在调试信号问题时，可以采用以下策略。
- 提高可见性。令调试器在收到信号时输出一条消息（例如，SIGUSR1），然后将此消息发送给程序。
    信号将在屏幕上留下明确的线索，这样更容易理解正在发生什么事情。示例：
    ```
    handle SIGUSR1 print nostop pass
    ```
- 禁用干扰性信号。信号处理程序可能对程序产生影响。可以令调试器忽略所有进入的信号，
    这样就不再调用信号处理程序。示例：
    ```
    handle SIGUSR1 noprint nostop nopass
    ```
- 引发bug。生成一个信号，并将其发送给程序，以测试它是否i触发bug。示例：
    ```
    signal SIGUSR1
    ```

可以通过如下命令打印所有信号和 GDB 被设置如何设置来处理它们。
- info signals
- info handle 

以下示例是一个简单的信号处理程序，它在程序处于内部延时循环繁忙期间计算信号SIGUSR1的接收频率。

```
#include <signal.h>
#include <stdio.h>
 
static int  num_sigusr1=0;
 
void handler(int sig) {
    num_sigusr1++;
    signal(SIGUSR1,&handler);
}

int main(int argc, char** argv) {
    int n,m, pol=0;
    signal(SIGUSR1,&handler);
    printf("- program starts\n"); fflush(stdout);
    for (n=0; n<10; n++) {
        raise(SIGUSR1);
        for (m=0; m<1000000000; m++)
            pol++;
    }
    printf("- program ends: received SIGUSR1 %d time(s)\n",
        num_sigusr1);
    fflush(stdout);
    return 0;
}
```

程序将向它自己发送10个SIGUSR1类型的信号。在程序运行期间，使用UNIX命令
`kill -USR1 <pid>`从另一个命令行解释器发送更多信号。在GDB中运行程序，
并使用GDB命令handle来配置GDB对信号的响应方式。首先，将信号输出到屏幕：

```
$ gcc -g3 -Wall -Wextra -o signal_catcher signal_catcher.c
$ gdb ./signal_catcher
(gdb) handle SIGUSR1 print nostop pass
Signal        Stop	Print	Pass to program	Description
SIGUSR1       No	Yes	Yes		User defined signal 1
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/c/signal_catcher/signal_catcher
- program starts
```

一旦处理程序接收到信号，GDB输出一条消息：

```
Program received signal SIGUSR1, User defined signal 1.

Program received signal SIGUSR1, User defined signal 1.

Program received signal SIGUSR1, User defined signal 1.
...
- program ends: received SIGUSR1 10 time(s)
[Inferior 1 (process 4777) exited normally]
(gdb)
```

接下来，禁用SIGUSR1，这样它不再到达程序：

```
(gdb) handle SIGUSR1 noprint nostop nopass
Signal        Stop	Print	Pass to program	Description
SIGUSR1       No	No	No		User defined signal 1
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/c/signal_catcher/signal_catcher
- program starts
- program ends: received SIGUSR1 0 time(s)
[Inferior 1 (process 5861) exited normally]
(gdb)
```

最后，使用GDB的signal命令来生成一个信号，以便测试程序如何响应：

```
(gdb) handle SIGUSR1 nostop pass
Signal        Stop	Print	Pass to program	Description
SIGUSR1       No	Yes	Yes		User defined signal 1
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/c/signal_catcher/signal_catcher
- program starts

Program received signal SIGUSR1, User defined signal 1.

Program received signal SIGUSR1, User defined signal 1.

Program received signal SIGUSR1, User defined signal 1.

Program received signal SIGUSR1, User defined signal 1.
^C
Program received signal SIGINT, Interrupt.
0x0000555555554845 in main (argc=1, argv=0x7fffffffde28) at signal_catcher.c:23
23	        for (m=0; m<1000000000; m++)
(gdb) signal SIGUSR1
Continuing with signal SIGUSR1.

Program received signal SIGUSR1, User defined signal 1.

Program received signal SIGUSR1, User defined signal 1.

Program received signal SIGUSR1, User defined signal 1.

Program received signal SIGUSR1, User defined signal 1.

Program received signal SIGUSR1, User defined signal 1.

Program received signal SIGUSR1, User defined signal 1.
- program ends: received SIGUSR1 11 time(s)
[Inferior 1 (process 6007) exited normally]
(gdb)
```

可以看到，处理程序接收到信号，并计算收到的次数。
