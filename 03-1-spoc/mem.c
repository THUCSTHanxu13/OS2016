#include <unistd.h>
#include <stdint.h>
#define MIN_ALLOC 1024 // minimum space allocate by program 1K
#define MIN_SBRK 65536 // minimum space allocate by lib 64K
#define STATUS_FREE  0
#define STATUS_USED  1

void *base = 0;
uint32_t len = 0;

inline uint32_t min(int32_t x, uint32_t y) {
    return x < y ? x : y;
}

// buddy system, space structure
// +-----+--------+------+
// | len | status | data |
// +-----+--------+------+
//   4b    4b       2^k-8

// undefined behavior when larger than 2GB memory used.


// recursive search for space
void* findSpace(void *base, uint32_t offset, uint32_t len, uint32_t target) {
    uint32_t* lptr = (uint32_t*)((char*)base + offset);
    uint32_t* sptr = (uint32_t*)((char*)base + offset + 4);
    void *tmp;

    if (len < target)
        return (void*)-1;
    else if (len == target) {
        if (*lptr == target && *sptr == STATUS_FREE) {
            *sptr = STATUS_USED;
            return (char*)base + offset;
        } else
            return (void*)-1;
    } else {
        if (*lptr == len) {
            if (*sptr == STATUS_USED) return (void*)-1;
            if (len == target) {
                return (char*)base + offset;
            } else {
                *lptr >>= 1;
                len >>= 1;
                *((uint32_t*)((char*)base + offset + len)) = len;
                *((uint32_t*)((char*)base + offset + len + 4)) = STATUS_FREE;
                return findSpace(base, offset, len, target);
            }
        } else {
            len >>= 1;
            tmp = findSpace(base, offset, len, target);
            if (tmp != (void*)-1) {
                return tmp;
            } else
                return findSpace(base, offset + len, len, target);
        }
    }
}

// free memory block and auto merge free space
void free_local(void *ptr) {
    uint32_t ll, offsetA, offsetB;

    ptr = (char*)ptr - 8;
    offsetA = (char*)ptr - (char*)base;
    ll = *(uint32_t*)ptr;
    *((uint32_t*)ptr + 1) = STATUS_FREE;
    while (ll < len) {
        offsetB = offsetA ^ ll;
        if (*(uint32_t*)((char*)base + offsetB) == ll &&
            *(uint32_t*)((char*)base + offsetB + 4) == STATUS_FREE) {
            offsetA = min(offsetA, offsetB);
            *(uint32_t*)((char*)base + offsetA) = ll * 2;
            ll *= 2;
        } else
            break;
    }
}

// return a pointer to memory
// use double size strategy
// no exception handling
void* malloc_local(uint32_t size) {
    void *ret;
    uint32_t target;

    //init
    if (len == 0) {
        base = sbrk(0);
        len = MIN_SBRK;
        sbrk(MIN_SBRK);
        *(uint32_t*)base = MIN_SBRK;
        *((uint32_t*)base + 1) = STATUS_FREE;
    }

    target = 1;
    while (target <= size + 8) target *= 2;
    if (target < MIN_ALLOC) target = MIN_ALLOC;

    while ((ret = findSpace(base, 0, len, target)) == (void*)-1) {
        sbrk(len);
        *(uint32_t*)((char*)base + len) = len;
        *(uint32_t*)((char*)base + len + 4) = STATUS_FREE;
        if (*(uint32_t*)base == len && *(uint32_t*)((char*)base + 4) == STATUS_FREE) {
            *(uint32_t*)base = len * 2;
        }
        len *= 2;
    }

    return (char*)ret + 8;
}
