/**
 * @file    tests/lib/string.c
 * @brief   Formated printing tests
 * @version 1.0
 * @date    04.02.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/assert.h>
#include <flos/debug.h>
#include <flos/string.h>

TEST(printf_test);

int printf_test() {
    char buf[10];

    sprintf(buf, "%d", 1);

    assert(!memcmp(buf, "1", 1), "buf is \"%s\"", buf);

    return 0;
}
