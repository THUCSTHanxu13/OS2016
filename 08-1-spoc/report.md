# 创建内核线程

###创建方法
在proc.c中
```
int pid3= kernel_thread(my_kern_thread, "this is my test kernel thread", 0);
myproc = find_proc(pid3);
set_proc_name(myproc, "my_kernel_proc");
cprintf("proc_init:: Created kernel thread my_kern_thread--> pid: %d, name: %s\n",myproc->pid, myproc->name);
```

###输出状态
在sched.c中输出所有线程状态

```
    last = (current == idleproc) ? &proc_list : &(current->list_link);
    le = last;
    do {
        if ((le = list_next(le)) != &proc_list) {
            next = le2proc(le, list_link);
            cprintf("inside shedule pid: %d, name: %s, state: %d\n",next->pid, next->name, next->state);
        }
    } while (le != last);

```

###结果

```
(THU.CST) os is loading ...

Special kernel symbols:
  entry  0xc010002a (phys)
  etext  0xc0107333 (phys)
  edata  0xc011ca68 (phys)
  end    0xc011d9d8 (phys)
Kernel executable memory footprint: 119KB
memory management: default_pmm_manager
e820map:
  memory: 0009fc00, [00000000, 0009fbff], type = 1.
  memory: 00000400, [0009fc00, 0009ffff], type = 2.
  memory: 00010000, [000f0000, 000fffff], type = 2.
  memory: 07efe000, [00100000, 07ffdfff], type = 1.
  memory: 00002000, [07ffe000, 07ffffff], type = 2.
  memory: 00040000, [fffc0000, ffffffff], type = 2.
default_init_memmap: nr free page is 32224
check_alloc_page() succeeded!
check_pgdir() succeeded!
check_boot_pgdir() succeeded!
-------------------- BEGIN --------------------
PDE(0e0) c0000000-f8000000 38000000 urw
  |-- PTE(38000) c0000000-f8000000 38000000 -rw
PDE(001) fac00000-fb000000 00400000 -rw
  |-- PTE(000e0) faf00000-fafe0000 000e0000 urw
  |-- PTE(00001) fafeb000-fafec000 00001000 -rw
--------------------- END ---------------------
use SLOB allocator
kmalloc_init() succeeded!
proc_init:: Created kernel thread init_main--> pid: 1, name: init1
proc_init:: Created kernel thread init_main--> pid: 2, name: init2
proc_init:: Created kernel thread my_kern_thread--> pid: 3, name: my_kernel_proc
++ setup timer interrupts
inside shedule pid: 1, name: init1, state: 2
inside shedule pid: 2, name: init2, state: 2
inside shedule pid: 3, name: my_kernel_proc, state: 2
 kernel_thread, pid = 1, name = init1
inside shedule pid: 2, name: init2, state: 2
inside shedule pid: 3, name: my_kernel_proc, state: 2
inside shedule pid: 1, name: init1, state: 2
 kernel_thread, pid = 2, name = init2
inside shedule pid: 3, name: my_kernel_proc, state: 2
inside shedule pid: 1, name: init1, state: 2
inside shedule pid: 2, name: init2, state: 2
 my_kernel_thread, pid = 3, name = my_kernel_proc
inside shedule pid: 1, name: init1, state: 2
inside shedule pid: 2, name: init2, state: 2
inside shedule pid: 3, name: my_kernel_proc, state: 2
 kernel_thread, pid = 1, name = init1 , arg  init main1: Hello world!! 
inside shedule pid: 2, name: init2, state: 2
inside shedule pid: 3, name: my_kernel_proc, state: 2
inside shedule pid: 1, name: init1, state: 2
 kernel_thread, pid = 2, name = init2 , arg  init main2: Hello world!! 
inside shedule pid: 3, name: my_kernel_proc, state: 2
inside shedule pid: 1, name: init1, state: 2
inside shedule pid: 2, name: init2, state: 2
 my_kernel_thread, pid = 3, name = my_kernel_proc , arg  this is my test kernel thread 
inside shedule pid: 1, name: init1, state: 2
inside shedule pid: 2, name: init2, state: 2
inside shedule pid: 3, name: my_kernel_proc, state: 2
 kernel_thread, pid = 1, name = init1 ,  en.., Bye, Bye. :)
 do_exit: proc pid 1 will exit
 do_exit: proc  parent c02ff008
inside shedule pid: 2, name: init2, state: 2
inside shedule pid: 3, name: my_kernel_proc, state: 2
inside shedule pid: 1, name: init1, state: 3
 kernel_thread, pid = 2, name = init2 ,  en.., Bye, Bye. :)
 do_exit: proc pid 2 will exit
 do_exit: proc  parent c02ff008
inside shedule pid: 3, name: my_kernel_proc, state: 2
inside shedule pid: 1, name: init1, state: 3
inside shedule pid: 2, name: init2, state: 3
 my_kernel_thread, pid = 3, name = my_kernel_proc ,  en.., Bye, Bye. :)
 do_exit: proc pid 3 will exit
 do_exit: proc  parent c02ff008
inside shedule pid: 1, name: init1, state: 3
inside shedule pid: 2, name: init2, state: 3
inside shedule pid: 3, name: my_kernel_proc, state: 3
do_wait: begin
do_wait: has kid find child  pid1
inside shedule pid: 2, name: init2, state: 3
inside shedule pid: 3, name: my_kernel_proc, state: 3
do_wait: begin
do_wait: has kid find child  pid2
inside shedule pid: 3, name: my_kernel_proc, state: 3
do_wait: begin
do_wait: has kid find child  pid3
do_wait: begin
100 ticks
qemu: terminating on signal 2
```