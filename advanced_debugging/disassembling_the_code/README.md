### 反汇编代码

gdb可用于进行运行时代码反汇编。在运行时反汇编代码的优势在于所有地址都已被加载程序解析，且这些地址基本已是最终地址。

以下gdb命令在运行时代码反汇编过程中非常实用：

- `set disassembly-flavor <intel | att> `（设置反汇编语法风格）
- `disassemble <函数名称>` （反汇编指定函数）

调用disassemble命令时，以下两个标志符特别有用：

- `/r` 标志要求同时以十六进制格式显示汇编指令
- `/m` 标志可以在汇编指令中穿插显示对应的 C/C++ 源代码（如果可用）

下面给出`/r`的示例：

```bash
$ cat answer.c
/* answer.c */
#include <stdio.h>

void runexperiment() {
    int *answerp;
    answerp = (void *) 42;
    printf("The answer is %d\n", *answerp);
}

void createplanet() {
    runexperiment();
}

int main() {
    printf("Hello Universe! Computing answer ...\n");
    fflush(stdout);
    createplanet();
    return 0;
}
$ gdb ./answer 
(gdb) set disassembly-flavor intel
(gdb) disassemble /r main
Dump of assembler code for function main:
   0x00000000000011d1 <+0>:     f3 0f 1e fa             endbr64
   0x00000000000011d5 <+4>:     55                      push   rbp
   0x00000000000011d6 <+5>:     48 89 e5                mov    rbp,rsp
   0x00000000000011d9 <+8>:     48 8d 05 40 0e 00 00    lea    rax,[rip+0xe40]        # 0x2020
   0x00000000000011e0 <+15>:    48 89 c7                mov    rdi,rax
   0x00000000000011e3 <+18>:    e8 88 fe ff ff          call   0x1070 <puts@plt>
   0x00000000000011e8 <+23>:    48 8b 05 21 2e 00 00    mov    rax,QWORD PTR [rip+0x2e21]        # 0x4010 <stdout@GLIBC_2.2.5>
   0x00000000000011ef <+30>:    48 89 c7                mov    rdi,rax
   0x00000000000011f2 <+33>:    e8 99 fe ff ff          call   0x1090 <fflush@plt>
   0x00000000000011f7 <+38>:    b8 00 00 00 00          mov    eax,0x0
   0x00000000000011fc <+43>:    e8 bb ff ff ff          call   0x11bc <createplanet>
   0x0000000000001201 <+48>:    b8 00 00 00 00          mov    eax,0x0
   0x0000000000001206 <+53>:    5d                      pop    rbp
   0x0000000000001207 <+54>:    c3                      ret
End of assembler dump.
(gdb) 
```

下面给出`/m`的示例：


```bash
(gdb) disassemble /m main
Dump of assembler code for function main:
19      int main() {
   0x00000000000011d1 <+0>:     endbr64
   0x00000000000011d5 <+4>:     push   rbp
   0x00000000000011d6 <+5>:     mov    rbp,rsp

20          printf("Hello Universe! Computing answer ...\n");
   0x00000000000011d9 <+8>:     lea    rax,[rip+0xe40]        # 0x2020
   0x00000000000011e0 <+15>:    mov    rdi,rax
   0x00000000000011e3 <+18>:    call   0x1070 <puts@plt>

21          fflush(stdout);
   0x00000000000011e8 <+23>:    mov    rax,QWORD PTR [rip+0x2e21]        # 0x4010 <stdout@GLIBC_2.2.5>
   0x00000000000011ef <+30>:    mov    rdi,rax
   0x00000000000011f2 <+33>:    call   0x1090 <fflush@plt>

22          createplanet();
   0x00000000000011f7 <+38>:    mov    eax,0x0
   0x00000000000011fc <+43>:    call   0x11bc <createplanet>

23          return 0;
   0x0000000000001201 <+48>:    mov    eax,0x0

24      }
   0x0000000000001206 <+53>:    pop    rbp
   0x0000000000001207 <+54>:    ret

End of assembler dump.
(gdb)
```

要合并这两个标志，请将它们一起输入（即 /rm），而非分别输入（即 /r /m）:

```bash
(gdb) disassemble /mr main
Dump of assembler code for function main:
19      int main() {
   0x00000000000011d1 <+0>:     f3 0f 1e fa             endbr64
   0x00000000000011d5 <+4>:     55                      push   rbp
   0x00000000000011d6 <+5>:     48 89 e5                mov    rbp,rsp

20          printf("Hello Universe! Computing answer ...\n");
   0x00000000000011d9 <+8>:     48 8d 05 40 0e 00 00    lea    rax,[rip+0xe40]        # 0x2020
   0x00000000000011e0 <+15>:    48 89 c7                mov    rdi,rax
   0x00000000000011e3 <+18>:    e8 88 fe ff ff          call   0x1070 <puts@plt>

21          fflush(stdout);
   0x00000000000011e8 <+23>:    48 8b 05 21 2e 00 00    mov    rax,QWORD PTR [rip+0x2e21]        # 0x4010 <stdout@GLIBC_2.2.5>
   0x00000000000011ef <+30>:    48 89 c7                mov    rdi,rax
   0x00000000000011f2 <+33>:    e8 99 fe ff ff          call   0x1090 <fflush@plt>

22          createplanet();
   0x00000000000011f7 <+38>:    b8 00 00 00 00          mov    eax,0x0
   0x00000000000011fc <+43>:    e8 bb ff ff ff          call   0x11bc <createplanet>

23          return 0;
   0x0000000000001201 <+48>:    b8 00 00 00 00          mov    eax,0x0

24      }
   0x0000000000001206 <+53>:    5d                      pop    rbp
   0x0000000000001207 <+54>:    c3                      ret

End of assembler dump.
```

#### 参考资料:
《高级C/C++编译技术》: 12.4.3 gdb（GNU调试器） 

