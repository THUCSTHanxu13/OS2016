#include <u.h>

enum {    // processor fault codes
  FMEM,   // bad physical address
  FTIMER, // timer interrupt
  FKEYBD, // keyboard interrupt
  FPRIV,  // privileged instruction
  FINST,  // illegal instruction
  FSYS,   // software trap
  FARITH, // arithmetic trap
  FIPAGE, // page fault on opcode fetch
  FWPAGE, // page fault on write
  FRPAGE, // page fault on read
  USER=16 // user mode exception
};

out(port, val)  { asm(LL,8); asm(LBL,16); asm(BOUT); }
ivec(void *isr) { asm(LL,8); asm(IVEC); }
stmr(int val)   { asm(LL,8); asm(TIME); }
halt(val)       { asm(LL,8); asm(HALT); }

trap(int *sp, int c, int b, int a, int fc, unsigned *pc)
{
  switch (fc) {
  case FKEYBD:
    asm(BIN);
    asm(PSHA);
    asm(POPB);
    asm(LI, 1);
    asm(BOUT);
    break;
  }
}

alltraps()
{
  asm(PSHA);
  asm(PSHB);
  asm(PSHC);
  asm(LUSP); asm(PSHA);
  trap();                // registers passed by reference/magic
  asm(POPA); asm(SUSP);
  asm(POPC);
  asm(POPB);
  asm(POPA);
  asm(RTI);
}

main()
{
  stmr(1000);
  ivec(alltraps);

  asm(STI);

  while (1) {
    asm(NOP);
  }

  halt(0);
}
