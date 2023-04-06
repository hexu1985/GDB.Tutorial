### 在GDB中检查数组

1. 在GDB中，可以输出整个数组，比如声明为：
    ```
    int x[25];
    ```
    方法是通过键入：
    ```
    (gdb) p x
    ```

    完整示例：
    ```
    $ gcc -g3 -Wall -Wextra -o test3 test3.c
    $ gdb test3
    (gdb) list
    1       #include <stdlib.h>
    2
    3       int main()
    4       {
    5           int x[25] = {0};
    6           x[3] = 12;
    7           (void) x;
    8
    9           return 0;
    10      }
    (gdb) start
    Temporary breakpoint 1 at 0x672: file test3.c, line 4.
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/test3/test3
    
    Temporary breakpoint 1, main () at test3.c:4
    4       {
    (gdb) next
    5           int x[25] = {0};
    (gdb) p x
    $1 = {0, 0, 0, 0, 9, 0, -136489376, 32767, -9192, 32767, 15775231, 0, 1, 0, 1431652109, 21845, -136430784, 32767, 0, 0, 1431652032, 21845, 1431651680, 21845, -8992}
    (gdb) next
    6           x[3] = 12;
    (gdb) p x
    $2 = {0 <repeats 25 times>}
    (gdb) next
    9           return 0;
    (gdb) p x
    $3 = {0, 0, 0, 12, 0 <repeats 21 times>}
    (gdb)
    ```

2. 但是，如果是动态创建的数组会是什么样呢？比如
    ```
    int *x;
    ...
    x = (int *) malloc( 25*sizeof(int) );
    ```
    如果要在GDB中输出数组，就不能键入
    ```
    (gdb) p x
    ```
    这只会简单打印数组地址。或者键入：
    ```
    (gdb) p *x
    ```
    这样只会输出数组的一个元素：x[0]。仍然可以像在命令`p x[5]`中那样输出单个元素，
    但不能简单地在x上使用`print`命令输出整个数组。
    
    完整示例：
    ```
    $ gcc -g3 -Wall -Wextra -o solutions_in_gdb solutions_in_gdb.c
    $ gdb solutions_in_gdb 
    (gdb) list
    1       #include <stdlib.h>
    2
    3       int *x;
    4
    5       int main()
    6       {
    7           x = (int *) malloc( 25*sizeof(int) );
    8           x[3] = 12;
    9
    10          return 0;
    (gdb) start
    Temporary breakpoint 1 at 0x64e: file solutions_in_gdb.c, line 7.
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/solutions_in_gdb/solutions_in_gdb
    
    Temporary breakpoint 1, main () at solutions_in_gdb.c:7
    7           x = (int *) malloc( 25*sizeof(int) );
    (gdb) next
    8           x[3] = 12;
    (gdb) next
    10          return 0;
    (gdb) p x
    $1 = (int *) 0x555555756260
    (gdb) p *x
    $2 = 0
    (gdb) p x[3]
    $3 = 12
    (gdb) p x[5]
    $4 = 0
    (gdb)
    ```

3. 无论是在C中声明为数组，还是指针，都可以用统一的方式打印几个在内存里连续的相同类型的对象。  
    用二进制操作符'@'将一个连续的内存区域作为伪数组,可以达到这个目的。'@'的左操作数应该是目标数组的第一个元素，  
    并且是一个单独的对象。右操作数应该是目标数组的长度。结果是整个数组的值，其元素都是左操作数的类型。  
    第一个元素是左操作数；第二个元素在内存中数紧邻着第一个元素，依此类推。下面是一个例子。如果程序
    ```
    int *array = (int *) malloc (len * sizeof (int));
    ```
    可以用下面命令打印数组的内容
    ```
    p *array@len
    ```
    '@'的左操作数必须存在于内存中。用'@'打印的数组值和用其它下标索引得到的值一样，并且会在表达式里强制转换为指针。

    完整示例：
    ```
    (gdb) list
    1       #include <stdlib.h>
    2
    3       int *x;
    4
    5       int main()
    6       {
    7           x = (int *) malloc( 25*sizeof(int) );
    8           x[3] = 12;
    9
    10          return 0;
    (gdb) start
    Temporary breakpoint 1 at 0x64e: file solutions_in_gdb.c, line 7.
    Starting program: /home/hexu/git/GDB.Tutorial/code/c/solutions_in_gdb/solutions_in_gdb
    
    Temporary breakpoint 1, main () at solutions_in_gdb.c:7
    7           x = (int *) malloc( 25*sizeof(int) );
    (gdb) next
    8           x[3] = 12;
    (gdb) p *x@25
    $1 = {0 <repeats 25 times>}
    (gdb) next
    10          return 0;
    (gdb) p *x@25
    $2 = {0, 0, 0, 12, 0 <repeats 21 times>}
    (gdb)
    ```

4. 另一种打印动态数组的方法是使用强制转化。转化会将一个值作为一个数组对待。比如：
    ```
    (gdb) p (int [25]) *x
    $3 = {0, 0, 0, 12, 0 <repeats 21 times>}
    ```
