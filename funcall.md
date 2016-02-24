###funcall中的堆栈有多大？是内核态堆栈还是用户态堆栈###

-	124MB，用户堆栈

###funcall中的全局变量ret放在内存中何处？如何对它寻址？###

-	global address中，直接寻址。

###funcall中的字符串放在内存中何处？如何对它寻址？###

-	90（传入给write的第二个参数），通过global address寻址

###局部变量i在内存中的何处？如何对它寻址？###

-	07bfffd8（单步进入函数后，在堆栈操作开取局部变量之后可获得），通过堆栈寻址

###当前系统是处于中断使能状态吗？###

-	不处于中断使能状态

###funcall中的函数参数是如何传递的？###

-	参数通过压栈传入到函数内部处理，从右向左压

###函数返回值是如何传递的？###

-	函数赋值给a寄存器返回

###注释###

```
root/usr/funcall.c  1: #include <u.h>
root/lib/u.h  1: // u.h
root/lib/u.h  2: 
root/lib/u.h  3: // instruction set
root/lib/u.h  4: enum {
root/lib/u.h  5:   HALT,ENT ,LEV ,JMP ,JMPI,JSR ,JSRA,LEA ,LEAG,CYC ,MCPY,MCMP,MCHR,MSET, // system
root/lib/u.h  6:   LL  ,LLS ,LLH ,LLC ,LLB ,LLD ,LLF ,LG  ,LGS ,LGH ,LGC ,LGB ,LGD ,LGF , // load a
root/lib/u.h  7:   LX  ,LXS ,LXH ,LXC ,LXB ,LXD ,LXF ,LI  ,LHI ,LIF ,
root/lib/u.h  8:   LBL ,LBLS,LBLH,LBLC,LBLB,LBLD,LBLF,LBG ,LBGS,LBGH,LBGC,LBGB,LBGD,LBGF, // load b
root/lib/u.h  9:   LBX ,LBXS,LBXH,LBXC,LBXB,LBXD,LBXF,LBI ,LBHI,LBIF,LBA ,LBAD,
root/lib/u.h  10:   SL  ,SLH ,SLB ,SLD ,SLF ,SG  ,SGH ,SGB ,SGD ,SGF ,                     // store
root/lib/u.h  11:   SX  ,SXH ,SXB ,SXD ,SXF ,
root/lib/u.h  12:   ADDF,SUBF,MULF,DIVF,                                                   // arithmetic
root/lib/u.h  13:   ADD ,ADDI,ADDL,SUB ,SUBI,SUBL,MUL ,MULI,MULL,DIV ,DIVI,DIVL,
root/lib/u.h  14:   DVU ,DVUI,DVUL,MOD ,MODI,MODL,MDU ,MDUI,MDUL,AND ,ANDI,ANDL,
root/lib/u.h  15:   OR  ,ORI ,ORL ,XOR ,XORI,XORL,SHL ,SHLI,SHLL,SHR ,SHRI,SHRL,
root/lib/u.h  16:   SRU ,SRUI,SRUL,EQ  ,EQF ,NE  ,NEF ,LT  ,LTU ,LTF ,GE  ,GEU ,GEF ,      // logical
root/lib/u.h  17:   BZ  ,BZF ,BNZ ,BNZF,BE  ,BEF ,BNE ,BNEF,BLT ,BLTU,BLTF,BGE ,BGEU,BGEF, // conditional
root/lib/u.h  18:   CID ,CUD ,CDI ,CDU ,                                                   // conversion
root/lib/u.h  19:   CLI ,STI ,RTI ,BIN ,BOUT,NOP ,SSP ,PSHA,PSHI,PSHF,PSHB,POPB,POPF,POPA, // misc
root/lib/u.h  20:   IVEC,PDIR,SPAG,TIME,LVAD,TRAP,LUSP,SUSP,LCL ,LCA ,PSHC,POPC,MSIZ,
root/lib/u.h  21:   PSHG,POPG,NET1,NET2,NET3,NET4,NET5,NET6,NET7,NET8,NET9,
root/lib/u.h  22:   POW ,ATN2,FABS,ATAN,LOG ,LOGT,EXP ,FLOR,CEIL,HYPO,SIN ,COS ,TAN ,ASIN, // math
root/lib/u.h  23:   ACOS,SINH,COSH,TANH,SQRT,FMOD,
root/lib/u.h  24:   IDLE
root/lib/u.h  25: };
root/lib/u.h  26: 
root/lib/u.h  27: // system calls
root/lib/u.h  28: enum {
root/lib/u.h  29:   S_fork=1, S_exit,   S_wait,   S_pipe,   S_write,  S_read,   S_close,  S_kill,
root/lib/u.h  30:   S_exec,   S_open,   S_mknod,  S_unlink, S_fstat,  S_link,   S_mkdir,  S_chdir,
root/lib/u.h  31:   S_dup2,   S_getpid, S_sbrk,   S_sleep,  S_uptime, S_lseek,  S_mount,  S_umount,
root/lib/u.h  32:   S_socket, S_bind,   S_listen, S_poll,   S_accept, S_connect, 
root/lib/u.h  33: };
root/lib/u.h  34: 
root/lib/u.h  35: typedef unsigned char uchar;
root/lib/u.h  36: typedef unsigned short ushort;
root/lib/u.h  37: typedef unsigned int uint;
root/lib/u.h  38: 
root/usr/funcall.c  2: 
root/usr/funcall.c  3: int ret;
root/usr/funcall.c  4: out(port, val)
root/usr/funcall.c  5: {
root/usr/funcall.c  6:   asm(LL,8);   // load register a with port
00000000  0000080e  LL    0x8 (D 8)
root/usr/funcall.c  7:   asm(LBL,16); // load register b with val
00000004  00001026  LBL   0x10 (D 16)
root/usr/funcall.c  8:   asm(BOUT);   // output byte to console
00000008  0000009a  BOUT
root/usr/funcall.c  9: }
root/usr/funcall.c  10: 
root/usr/funcall.c  11: int write(int f, char *s, int n)
0000000c  00000002  LEV   0x0 (D 0)
root/usr/funcall.c  12: {
root/usr/funcall.c  13:   int i;
root/usr/funcall.c  14:   ret = 1;
00000010  fffff801  ENT   0xfffffff8 (D -8)	#在栈内开空间保存现场
00000014  00000123  LI    0x1 (D 1)			#1赋值a
00000018  00000045  SG    0x0 (D 0)			#a赋值给ret
root/usr/funcall.c  15:   i=n;				#循环准备
0000001c  0000200e  LL    0x20 (D 32)
00000020  00000440  SL    0x4 (D 4)
root/usr/funcall.c  16:   while (i--)
00000024  00000003  JMP   <fwd>				#等着回填
root/usr/funcall.c  17:     out(f, *s++);
00000028  0000180e  LL    0x18 (D 24)
0000002c  ffffff57  SUBI  0xffffffff (D -1)		#循环变量修改
00000030  00001840  SL    0x18 (D 24)
00000034  ffffff1f  LXC   0xffffffff (D -1)
00000038  0000009d  PSHA						#压入参数给out
0000003c  0000180e  LL    0x18 (D 24)
00000040  0000009d  PSHA						#压入参数给out
00000044  ffffb805  JSR   0xffffffb8 (TO 0x0)	#调用输出函数输出
00000048  00001001  ENT   0x10 (D 16)			
root/usr/funcall.c  18:   return i;
0000004c  0000040e  LL    0x4 (D 4)
00000050  00000157  SUBI  0x1 (D 1)
00000054  00000440  SL    0x4 (D 4)
00000058  00000154  ADDI  0x1 (D 1)
0000005c  00000086  BNZ   <fwd>					#等着回填
00000060  0000040e  LL    0x4 (D 4)
00000064  00000802  LEV   0x8 (D 8)
root/usr/funcall.c  19: }  
root/usr/funcall.c  20: 
root/usr/funcall.c  21: main()
00000068  00000802  LEV   0x8 (D 8)
root/usr/funcall.c  22: {
root/usr/funcall.c  23: 
root/usr/funcall.c  24:   //Change S1/S2 ID to your student ID, and change 12 to new str length
root/usr/funcall.c  25:   ret = write(1, "2013011343 2013011344\n",22);
0000006c  0000169e  PSHI  0x16 (D 22)   #压入22
00000070  00000008  LEAG  0x0 (D 0)		#把字符串地址传入a
00000074  0000009d  PSHA				#把字符串地址压入栈
00000078  0000019e  PSHI  0x1 (D 1)		#压入1
0000007c  ffff9005  JSR   0xffffff90 (TO 0x10)	#调用write
00000080  00001801  ENT   0x18 (D 24)	#堆栈恢复
00000084  00000045  SG    0x0 (D 0)		#将返回值a赋值给ret，ret通过global address的访问
root/usr/funcall.c  26:   asm(HALT);	#程序结束
00000088  00000000  HALT
root/usr/funcall.c  27: }
root/usr/funcall.c  28: 
0000008c  00000002  LEV   0x0 (D 0)
```