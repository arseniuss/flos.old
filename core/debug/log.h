/**
 * @file    debug/log.h
 * @brief   Logging header
 * @version 1.0
 * @date    14.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_debug__LOG_H__
#    define __flos_debug__LOG_H__

typedef int (log_puts_t)(const char *);

extern log_puts_t *kernel_log_handle;

#endif /* __flos_debug__LOG_H__ */
