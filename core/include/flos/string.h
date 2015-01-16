/**
 * @file    include/flos/string.h    
 * @brief   String functions
 * @version 1.0
 * @date    14.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__STRING_H__
#    define __flos__STRING_H__

#    include <flos/defs.h>
#    include <flos/types.h>
#    include <flos/vaargs.h>

__linkage size_t strlen(const char *s);
__linkage size_t strnlen(const char *s, size_t count);
__linkage int memcmp(const void * s1, const void * s2, size_t n);
__linkage void *memcpy(void * dst, const void * src, size_t n);
__linkage void * memset(void *dst, int c, unsigned int n);

__linkage int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
__linkage int sprintf(char *buf, const char *fmt, ...);
__linkage int snprintf(char *buf, size_t size, const char *fmt, ...);
__linkage int vsprintf(char *buf, const char *fmt, va_list args);

__linkage int ffs(int);
__linkage int fls(int);

#endif /* __flos__STRING_H__ */
