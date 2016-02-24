#os0
使用`./xc -s -Ilinux -Iroot/lib root/usr/os/os0.c`可以获得汇编代码。

我们删除了无效的内容，留下了如下汇编代码，并进行了注释。

```
## out(port, val)  { asm(LL,8); asm(LBL,16); asm(BOUT); }
00000000  0000080e  LL    0x8 (D 8)
00000004  00001026  LBL   0x10 (D 16)
00000008  0000009a  BOUT
0000000c  00000002  LEV   0x0 (D 0)
00000010  0000080e  LL    0x8 (D 8)

## ivec(void *isr) { asm(LL,8); asm(IVEC); }
00000014  000000a4  IVEC
00000018  00000002  LEV   0x0 (D 0)
0000001c  0000080e  LL    0x8 (D 8)

## stmr(int val)   { asm(LL,8); asm(TIME); }
00000020  000000a7  TIME
00000024  00000002  LEV   0x0 (D 0)
00000028  0000080e  LL    0x8 (D 8)

## halt(val)       { asm(LL,8); asm(HALT); }
0000002c  00000000  HALT
00000030  00000002  LEV   0x0 (D 0)

## alltraps()      { asm(PSHA); asm(PSHB); current++; asm(POPB); asm(POPA); asm(RTI); }
00000034  0000009d  PSHA
00000038  000000a0  PSHB
0000003c  00000015  LG    0x0 (D 0)
00000040  ffffff57  SUBI  0xffffffff (D -1)
00000044  00000045  SG    0x0 (D 0)
00000048  000000a1  POPB
0000004c  000000a3  POPA
00000050  00000098  RTI 
00000054  00000002  LEV   0x0 (D 0)

## main()
### current = 0;
00000058  00000023  LI    0x0 (D 0)
0000005c  00000045  SG    0x0 (D 0)
### stmr(1000);
00000060  0003e89e  PSHI  0x3e8 (D 1000)
00000064  ffffb405  JSR   0xffffffb4 (TO 0x1c)
00000068  00000801  ENT   0x8 (D 8)
### ivec(alltraps);
0000006c  ffffc408  LEAG  0xffffffc4 (D -60)
00000070  0000009d  PSHA
00000074  ffff9805  JSR   0xffffff98 (TO 0x10)
00000078  00000801  ENT   0x8 (D 8)
### asm(STI);
0000007c  00000097  STI 

### while (current < 10) { if (current & 1) out(1, '1'); else out(1, '0'); }
00000080  00000003  JMP   <fwd>
00000084  00000015  LG    0x0 (D 0)
00000088  00000169  ANDI  0x1 (D 1)
0000008c  00000084  BZ    <fwd>
00000090  0000319e  PSHI  0x31 (D 49)
00000094  0000019e  PSHI  0x1 (D 1)
00000098  ffff6405  JSR   0xffffff64 (TO 0x0)
0000009c  00001001  ENT   0x10 (D 16)
000000a0  00000003  JMP   <fwd>
000000a4  0000309e  PSHI  0x30 (D 48)
000000a8  0000019e  PSHI  0x1 (D 1)
000000ac  ffff5005  JSR   0xffffff50 (TO 0x0)
000000b0  00001001  ENT   0x10 (D 16)
000000b4  00000015  LG    0x0 (D 0)
000000b8  00000a3b  LBI   0xa (D 10)
000000bc  0000008c  BLT   <fwd>

### halt(0);
000000c0  0000009e  PSHI  0x0 (D 0)
000000c4  ffff6005  JSR   0xffffff60 (TO 0x28)
000000c8  00000801  ENT   0x8 (D 8)

000000cc  00000002  LEV   0x0 (D 0)
```

###何处设置的中断使能？
如下汇编代码开启中断使能
```
### asm(STI);
0000007c  00000097  STI 
```

###系统何时处于中断屏蔽状态？
在`00000007`之前。在中断处理的过程中。

###如果系统处于中断屏蔽状态，如何让其中断使能？
使用`STI`命令

###系统产生中断后，CPU会做哪些事情？（在没有软件帮助的情况下）
跳转到之前设置的中断处理地址。
`ivec(void *isr)`函数用来设置中断处理的函数地址。

###CPU执行RTI指令的具体完成工作是哪些？

