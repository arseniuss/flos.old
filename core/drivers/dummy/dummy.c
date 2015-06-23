/**
 * @file    drivers/dummy/main.c
 * @brief   Dummy driver
 * @version 0.1
 * @date    23.06.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/module.h>

int dummy_init() {
    return 0;
}

void dummy_exit() {

}

struct module dummy_module = {
    .name = "dummy",
    .init = &dummy_init,
    .exit = &dummy_exit
};

EXTERNAL_MODULE(dummy_module);
