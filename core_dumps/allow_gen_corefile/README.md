### 允许生成core文件

**coredump文件的存储位置**

我们知道在Linux系统中，如果进程崩溃了，系统内核会捕获到进程崩溃信息，然后将进程的coredump 信息写入到文件中，这个文件名默认是core 。
存储位置与对应的可执行程序在同一目录下，文件名是core，大家可以通过下面的命令看到core文件的存在位置：

```
$ cat /proc/sys/kernel/core_pattern
/home/hexu/corefile/core-%e-%p-%t
```

Core_pattern的格式：
- ％p 转储过程的PID
- ％u （数字）转储进程的实际UID
- ％G （数字）转储过程的实际GID
- ％s 引起转储的信号数
- ％t 转储时间，表示为自1970年1月1日00:00:00 +0000（UTC）以来的秒数
- ％H 主机名（与uname（2）返回的节点名相同）
- ％e 可执行文件名（无路径前缀）
- ％E 可执行文件的路径名，用斜杠（’/’）替换为感叹号（’！’）。
- ％C 崩溃过程的核心文件大小软资源限制（自Linux 2.6.24开始）

**查看是否打开了coredump**

通过ulimit -c命令可以查看当前是否限制core文件大小

```
$ ulimit -c
0
```

如果返回0，说明阻止生成core文件

可以使用下面命令修改：

```
$ ulimit -c unlimited
```

但是这只对当前shell会话生效。

如果想要永久修改，可以通过修改/etc/security/limits.conf文件来完成：

```
$ sudo vim /etc/security/limits.conf
```

添加两行：
- `* soft  core unlimited`
- `* hard  core unlimited`

```
#<domain>      <type>  <item>         <value>
#

#*               soft    core            0
#root            hard    core            100000
#*               hard    rss             10000
#@student        hard    nproc           20
#@faculty        soft    nproc           20
#@faculty        hard    nproc           50
#ftp             hard    nproc           0
#ftp             -       chroot          /ftp
#@student        -       maxlogins       4
*                soft    core            unlimited
*                hard    core            unlimited
```

**配置core文件生成路径**

1. 首先在根目录下建立一个储存coredump文件的文件夹，命令如下：

```
$ mkdir /corefile
```

2. 设置产生coredump文件的大小，命令如下：

```
$ ulimit -c unlimited
```

永久方法见上文。


3. 设置生成 core文件的名称和生成路径

执行以下两条命令：

```
$ sudo bash -c "echo '1' > /proc/sys/kernel/core_uses_pid"
$ sudo bash -c "echo '/corefile/core-%e-%p-%t' > /proc/sys/kernel/core_pattern"
```

将coredump产生的格式制定好写入core_pattern文件，这样当coredump产生时会直接以这种格式存入到根目录下的文件夹corefile中。

如果想要永久修改，可以通过修改/etc/sysctl.conf文件来完成：

```
$ sudo bash -c "sysctl -w 'kernel.core_uses_pid=1' >>/etc/sysctl.conf"
$ sudo bash -c "sysctl -w 'kernel.core_pattern=/corefile/core-%e-%p-%t' >>/etc/sysctl.conf"
$ sudo sysctl -p # (查看生效参数，验证设置是否生效)
kernel.core_uses_pid = 1
kernel.core_pattern = /corefile/core-%e-%p-%t
```

4. 关闭apport.service，修改/etc/default/apport 

把/etc/default/apport的enabled修改为0就ok了。

```
# set this to 0 to disable apport, or to 1 to enable it
# you can temporarily override this with
# sudo service apport start force_start=1
enabled=0
```

