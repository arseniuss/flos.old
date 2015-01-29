/**
 * @file    include/flos/vaargs.h    
 * @brief   Variadical arguments
 * @version 1.0
 * @date    13.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__VAARGS_H__
#    define __flos__VAARGS_H__

typedef char *va_list;

#    define __va_size(type)                                                    \
	(((sizeof(type) + sizeof(long) - 1) / sizeof(long)) * sizeof(long))

#    define va_start(ap, last)                                                 \
	((ap) = (va_list)&(last) + __va_size(last))

#    define va_arg(ap, type)                                                   \
	(*(type *)((ap) += __va_size(type), (ap) - __va_size(type)))

#    define va_end(ap)  ((void)0)

#endif /* __flos__VAARGS_H__ */
