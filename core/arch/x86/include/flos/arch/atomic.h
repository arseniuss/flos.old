/**
 * @file    arch/x86/include/flos/arch/atomic.h
 * @brief   Atomic operations
 * @version 1.0
 * @date    10.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__ATOMIC_H__
#    define __flos_x86__ATOMIC_H__

#    include <flos/types.h>
#    include <flos/defs.h>

typedef struct {
    volatile u32 counter;
} atomic_t;

#    define ATOMIC_INIT(v)          {.counter = v}

static inline u32 atomic_read(atomic_t * a) {
    return a->counter;
}

static inline void set_bit(u bit, atomic_t * a) {
  __asm("lock orl %1, %0;": "=m"(a->counter):"r"(0x1 << bit));
}

static inline void clear_bit(u bit, atomic_t * a) {
  __asm("lock andl %1, %0;": "=m"(a->counter):"r"(~(0x1 << bit)));
}

static inline void atomic_set(atomic_t * a, u32 v) {
  __asm("lock movl %1, %0;": "=m"(a->counter):"r"(v));
}

static inline void atomic_add(atomic_t * a, u32 v) {
  __asm("lock addl %1, %0;": "=m"(a->counter):"r"(v));
}

static inline void atomic_sub(atomic_t * a, u32 v) {
  __asm("lock subl %1, %0;": "=m"(a->counter):"r"(v));
}

static inline void atomic_inc(atomic_t * a) {
  __asm("lock incl %0;":"=m"(a->counter));
}

static inline void atomic_dec(atomic_t * a) {
  __asm("lock decl %0;":"=m"(a->counter));
}

#endif
