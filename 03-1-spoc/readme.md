###malloc/free 实现

首先我们发现了如下特性

> The current Mac OS X implementation of sbrk is an emulation, and has a maximum allocation of 4 megabytes.[3] When this limit is reached, −1 is returned and the errno is set to ENOMEM.

----
我们实现了一个简单的内存管理库，采用了buddy system的策略。

**向系统申请内存**

在首次使用首先使用`sbrk(0)`获取堆栈的第地址。我们将向系统申请内存的最小单位定为`#define MIN_SBRK 65536 //64K`，之后采用倍增的方式管理。

**内存块的格式**

我们采用了如下格式存储被用户程序alloc的内存块，可以看到，我们的结构消耗了额外的8字节内存。

```
buddy system, space structure
+-----+--------+------+
| len | status | data |
+-----+--------+------+
  4b    4b       2^k-8

```

**应用程序调用malloc**

首先确定`2^k-8 >= requirement`，随后递归的寻找`2^k`大小的内存块。不难发现递归函数传递的参数和上述结构中存储的信息足够还原buddy system中的层次结构。具体实现细节在此不赘述。

**应用程序调用free**

首先修改当前块的`status`，然后按照层次关系自底向上的合并空闲块。