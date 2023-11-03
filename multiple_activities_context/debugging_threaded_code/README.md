### 调试多线程代码

在某些操作系统里，一个程序可能有多个线程。线程精确概念随着各个操作系统而不一样，
但大体上，一个有多个线程的进程和多进程相似，除了多线程共享一个地址空间（就是说，他们可以检查和修改同一个变量）。
另一方面，每个线程有它自己的寄存器和执行栈，也可能有自己私有的存储空间。

GDB 提供了多个调试多线程的工具：
- 新线程的自动通知
- ‘thread threadno’，切换线程
- ‘info threads’，查询线程
- ‘thread apply [threadno] [all] args’，对线程列表执行命令

GDB 线程调试工具可以观察进程的所有线程，而一旦 GDB 控制线程的话，这个线程就总是调试的焦点了。
这个线程称为当前线程。调试命令从当前线程的角度来显示进程的信息。
一旦 GDB 察觉到进程的新线程，GDB 就会用‘[New systag]’的方式显示目标系统的标识。
systag 是线程的标识，各个系统不一样。例如，当 GDB 发现一个新线程的时候，在 GNU/Linux 你可能看到

```
$ gcc -g3 -Wall -Wextra  -o sieve sieve.c -lpthread -lm
$ gdb sieve
(gdb) run 100 2
Starting program: /home/hexu/git/GDB.Tutorial/code/c/sieve/sieve 100 2
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
[New Thread 0x7ffff7424700 (LWP 23079)]
[Thread 0x7ffff7424700 (LWP 23079) exited]
[New Thread 0x7ffff6c23700 (LWP 23080)]
4 values of base done
0 values of base done
4 total values of base done
the number of primes found was 25
[Thread 0x7ffff6c23700 (LWP 23080) exited]
[Inferior 1 (process 23075) exited normally]
(gdb)
```

出于调试的目的，GDB 自己会给线程一个编号–总是一个整数。
`info threads`
显示当前进程里的线程的总概要。GDB 显示每个线程（以此为序）：
1. GDB 分配的线程号
2. 目标系统的线程标识（systag）
3. 线程当前栈的概要
线程号左边的星号’*'代表此线程是当前线程。例如：

```
$ gcc -g3 -Wall -Wextra  -o sieve sieve.c -lpthread -lm
$ gdb sieve
(gdb) run 100000000 3
Starting program: /home/hexu/git/GDB.Tutorial/code/c/sieve/sieve 100000000 3
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
[New Thread 0x7ffff7424700 (LWP 24020)]
[New Thread 0x7ffff6c23700 (LWP 24021)]
[New Thread 0x7ffff6422700 (LWP 24022)]
^C
Thread 1 "sieve" received signal SIGINT, Interrupt.
0x00007ffff7bbcd2d in __GI___pthread_timedjoin_ex (threadid=140737341703936, thread_return=0x7fffffffdd20, abstime=0x0, block=<optimized out>) at pthread_join_common.c:89
89	pthread_join_common.c: 没有那个文件或目录.
(gdb) info threads
  Id   Target Id         Frame
* 1    Thread 0x7ffff7fa3740 (LWP 24019) "sieve" 0x00007ffff7bbcd2d in __GI___pthread_timedjoin_ex (threadid=140737341703936, thread_return=0x7fffffffdd20, abstime=0x0,
    block=<optimized out>) at pthread_join_common.c:89
  2    Thread 0x7ffff7424700 (LWP 24020) "sieve" 0x0000555555554911 in crossout (k=17) at sieve.c:37
  3    Thread 0x7ffff6c23700 (LWP 24021) "sieve" 0x000055555555491c in crossout (k=13) at sieve.c:36
  4    Thread 0x7ffff6422700 (LWP 24022) "sieve" 0x0000555555554923 in crossout (k=11) at sieve.c:36
(gdb) bt
#0  0x00007ffff7bbcd2d in __GI___pthread_timedjoin_ex (threadid=140737341703936, thread_return=0x7fffffffdd20, abstime=0x0, block=<optimized out>) at pthread_join_common.c:89
#1  0x0000555555554b11 in main (argc=3, argv=0x7fffffffde18) at sieve.c:86
```

可以通过切换到其他任何线程然后执行bt命令来检查该线程的栈。
`thread threadno`将 threadno 指向的线程设置为当前线程。这个命令的参数 threadno 是 GDB 内部的线程号，
就是’info threads’命令显示第一列。
GDB 会显示你选择的线程的系统标识和它当前栈的概要：

例如，我想看看线程3的调用栈：
```
(gdb) thread 3
[Switching to thread 3 (Thread 0x7ffff6c23700 (LWP 24021))]
#0  0x000055555555491c in crossout (k=13) at sieve.c:36
36	    for (i = k; i*k <= n; i++)  {
(gdb) bt
#0  0x000055555555491c in crossout (k=13) at sieve.c:36
#1  0x00005555555549e5 in worker (tn=1) at sieve.c:59
#2  0x00007ffff7bbb6db in start_thread (arg=0x7ffff6c23700) at pthread_create.c:463
#3  0x00007ffff754661f in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95
(gdb)
```

`thread apply`命令可以让你在一个或多个线程上执行名为command命令。用参数threadno指定你希望操作的线程数目。
可以是单个线程号，’info threads’显示的第一列；或者可以是线程范围，像 2-4.要操作所有线程，敲入 thread apply all command。
命令格式为：thread apply [threadno] [all] command

例如，我想一次看到所有线程的调用栈：
```
(gdb) thread apply all bt

Thread 4 (Thread 0x7ffff6422700 (LWP 24022)):
#0  0x0000555555554923 in crossout (k=11) at sieve.c:36
#1  0x00005555555549e5 in worker (tn=2) at sieve.c:59
#2  0x00007ffff7bbb6db in start_thread (arg=0x7ffff6422700) at pthread_create.c:463
#3  0x00007ffff754661f in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95

Thread 3 (Thread 0x7ffff6c23700 (LWP 24021)):
#0  0x000055555555491c in crossout (k=13) at sieve.c:36
#1  0x00005555555549e5 in worker (tn=1) at sieve.c:59
#2  0x00007ffff7bbb6db in start_thread (arg=0x7ffff6c23700) at pthread_create.c:463
#3  0x00007ffff754661f in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95

Thread 2 (Thread 0x7ffff7424700 (LWP 24020)):
#0  0x0000555555554911 in crossout (k=17) at sieve.c:37
#1  0x00005555555549e5 in worker (tn=0) at sieve.c:59
#2  0x00007ffff7bbb6db in start_thread (arg=0x7ffff7424700) at pthread_create.c:463
#3  0x00007ffff754661f in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95

Thread 1 (Thread 0x7ffff7fa3740 (LWP 24019)):
#0  0x00007ffff7bbcd2d in __GI___pthread_timedjoin_ex (threadid=140737341703936, thread_return=0x7fffffffdd20, abstime=0x0, block=<optimized out>) at pthread_join_common.c:89
#1  0x0000555555554b11 in main (argc=3, argv=0x7fffffffde18) at sieve.c:86
(gdb)
```

最后，给出与线程相关的GDB命令用法汇总：
- info threads（给出关于当前所有线程的信息）；
- thread 3（改成线程3）；
- break 88 thread 3（当线程3到达源代码行88时停止执行）；
- break 88 thread 3 if x == y（当线程3到达源代码行88，并且变量x和y相等时停止执行）；
- thread apply all bt（打印当前所有线程的调用栈信息）。
