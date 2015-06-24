/**
 * @file    module/module.h
 * @brief   Module subsystem header
 * @version 1.0
 * @date    25.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_module__MODULE_H__
#    define __flos_module__MODULE_H__

#    include <flos/list.h>

/** list of kernel modules */
extern struct list_head modules;

/** list of module type handlers */
extern struct list_head handlers;

#endif /* __flos_module__MODULE_H__ */
