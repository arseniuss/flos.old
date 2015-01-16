/**
 * @file    include/flos/assert.h    
 * @brief   Assertation in kernel
 * @version 1.0
 * @date    14.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__ASSERT_H__
#    define __flos__ASSERT_H__

#    include <flos/kprintf.h>

#    define assert(x, ...)                                                     \
        do { if(!(x)) {                                                        \
            kcritf("!"#x" in %s @ %s:%d\n", ##__VA_ARGS__, __func__,           \
                __FILE__, __LINE__);                                           \
            while(1);                                                          \
        } } while(0)

#    define static_assert(x, ...)                                              \
        typedef char static_assertion_ ## __VA_ARGS__ [(x)?1:-1]

#endif /* !__flos__ASSERT_H__ */
