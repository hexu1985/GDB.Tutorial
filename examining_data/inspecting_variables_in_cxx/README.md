### 在GDB中检查C++变量

对于C++程序，print命令同样起作用，但输出略有不同。  
为了说明C++代码的情况，我们使用前面用过的二叉树示例的C++版本。  

```
$ g++ -g3 -Wall -Wextra -o bintree bintree.cc
$ gdb bintree
(gdb) break 37
Breakpoint 1 at 0x997: file bintree.cc, line 37.
(gdb) set args 12 8 5 19 16
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/bintree++/bintree 12 8 5 19 16

Breakpoint 1, node::insert (x=8) at bintree.cc:37
37            if (x < tmp->val)
(gdb) p *tmp
$1 = {static root = 0x555555768e70, val = 12, left = 0x0, right = 0x0}
(gdb) p *root
$2 = {static root = 0x555555768e70, val = 12, left = 0x0, right = 0x0}
(gdb) ptype node
type = void (node * const, int)
(gdb) ptype class node
type = class node {
  public:
    static node *root;
    int val;
    node *left;
    node *right;

    node(int);
    static void insert(int);
    static void printtree(node *);
}
(gdb) c
Continuing.

Breakpoint 1, node::insert (x=5) at bintree.cc:37
37             if (x < tmp->val)
(gdb) p *tmp
$3 = {static root = 0x555555768e70, val = 12, left = 0x555555768e90, right = 0x0}
(gdb) p *root
$5 = {static root = 0x555555768e70, val = 12, left = 0x555555768e90, right = 0x0}
(gdb) c
Continuing.

Breakpoint 1, node::insert (x=5) at bintree.cc:37
37             if (x < tmp->val)
(gdb) p *tmp
$6 = {static root = 0x555555768e70, val = 8, left = 0x0, right = 0x0}
(gdb) p *root
$7 = {static root = 0x555555768e70, val = 12, left = 0x555555768e90, right = 0x0}
(gdb)
```

我们可以看到`p *tmp`类似于C程序的情况，只是现在还输出了类node的static变量root的值。  
另外GDB的ptype命令可以很方便地快速浏览类的结构，通过命令`ptype class node`。

如果结构体的成员比较多，这种显示会显得杂乱无章，不方便查看每一个成员的数据，  
GDB还提供了一个使显示更加漂亮的选项，命令为set print pretty。
```
$ g++ -g3 -Wall -Wextra -o bintree bintree.cc
$ gdb bintree
(gdb) break 37
Breakpoint 1 at 0x997: file bintree.cc, line 37.
(gdb) set args 12 8 5 19 16
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/bintree++/bintree 12 8 5 19 16

Breakpoint 1, node::insert (x=8) at bintree.cc:37
37              if (x < tmp->val)
(gdb) p *tmp
$1 = {static root = 0x555555768e70, val = 12, left = 0x0, right = 0x0}
(gdb) c
Continuing.

Breakpoint 1, node::insert (x=5) at bintree.cc:37
37              if (x < tmp->val)
(gdb) p *tmp
$2 = {static root = 0x555555768e70, val = 12, left = 0x555555768e90, right = 0x0}
(gdb) set print pretty
(gdb) show print pretty
Pretty formatting of structures is on.
(gdb) p *tmp
$3 = {
  static root = 0x555555768e70,
  val = 12,
  left = 0x555555768e90,
  right = 0x0
}
(gdb)
```

还有，需要记住，GDB需要根据与C++使用的相同作用域规则来指定变量。例如：

```
$ g++ -g3 -Wall -Wextra -o bintree bintree.cc
$ gdb bintree
(gdb) break 74
Breakpoint 1 at 0xad1: file bintree.cc, line 74.
(gdb) set args 12 8 5 19 16
(gdb) run
Starting program: /home/hexu/git/GDB.Tutorial/code/cxx/bintree++/bintree 12 8 5 19 16

Breakpoint 1, main (argc=6, argv=0x7fffffffdcc8) at bintree.cc:74
74          node::printtree(node::root);
(gdb) p root
$1 = (struct hdr *) 0x0
(gdb) p *root
Cannot access memory at address 0x0
(gdb) p node::root
$2 = (node *) 0x555555768e70
(gdb) p *node::root
$3 = {static root = 0x555555768e70, val = 12, left = 0x555555768e90, right = 0x555555768ed0}
(gdb)
```

### 参考资料:
- 《THE ART OF DEBUGGING with GDB, DDD, and Eclipse》: 3.2.5 What About C++?
