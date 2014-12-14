/**
 * @file    lib/ctype.c  
 * @brief   Character type
 * @version 1.0
 * @date    14.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/ctype.h>

const unsigned char
_ctype[] = {
            _C, _C, _C, _C, _C, _C, _C, _C,
            _C, _C | _S, _C | _S, _C | _S, _C | _S, _C | _S, _C, _C,
            _C, _C, _C, _C, _C, _C, _C, _C,
            _C, _C, _C, _C, _C, _C, _C, _C,
            _S | _SP, _P, _P, _P, _P, _P, _P, _P,
            _P, _P, _P, _P, _P, _P, _P, _P,
            _D, _D, _D, _D, _D, _D, _D, _D,
            _D, _D, _P, _P, _P, _P, _P, _P,
            _P, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U,
            _U, _U, _U, _U, _U, _U, _U, _U,
            _U, _U, _U, _U, _U, _U, _U, _U,
            _U, _U, _U, _P, _P, _P, _P, _P,
            _P, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L,
            _L, _L, _L, _L, _L, _L, _L, _L,
            _L, _L, _L, _L, _L, _L, _L, _L,
            _L, _L, _L, _P, _P, _P, _P, _C,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            _S | _SP, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P,
            _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P,
            _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U,
            _U, _U, _U, _U, _U, _U, _U, _P, _U, _U, _U, _U, _U, _U, _U, _L,
            _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L,
            _L, _L, _L, _L, _L, _L, _L, _P, _L, _L, _L, _L, _L, _L, _L, _L
};
