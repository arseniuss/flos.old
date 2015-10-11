/**
 * @file    include/flos/types.h
 * @brief   Type definitions used in flos core
 * @version 1.0
 * @date    29.11.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__TYPES_H__
#    define __flos__TYPES_H__

#    define NULL        0              ///< null value
#    define NULL_p      ((void *)0)    ///< null pointer
#    define MAX         ((unsigned)-1) ///< max value
#    define MAX_p       ((void *)MAX)  ///< max pointer

#    if __CHAR_BIT__ == 8
typedef unsigned char u8;       ///< unsigned 8bit int type
typedef signed char s8;         ///< signed 8bit int type
#    else
#        error A 'char' lenght is not 8 bits!
#    endif

#    define u8_min      0              ///< min unsigned 8bit int value
#    define u8_max      0xff           ///< max unsigned 8bit int value
#    define s8_min      (-0x7f - 1)    ///< min signed 8bit int value
#    define s8_max      0x7f           ///< max signed 8bit int value

/**
 * @def char_max
 * @short max char value
 *
 * @def char_min
 * @short min char value
 */
#    if __CHAR_UNSIGNED__
#        define char_max    u8_max
#        define char_min    u8_min
#    else
#        define char_max    s8_max
#        define char_min    s8_min
#    endif

/**
 * @typedef u16
 * @short unsigned 16bit int type
 *
 * @typedef s16
 * @short signed 16bit int type
 */
#    if __SIZEOF_INT__ == 2
typedef unsigned int u16;
typedef signed int s16;
#    elif __SIZEOF_SHORT__ == 2
typedef unsigned short u16;
typedef signed short s16;
#    else
#        error Neither 'int' not 'short' is 2 bytes long!
#    endif

#    define u16_min     0              ///< min unsigned 16bit value
#    define u16_max     0xffffU        ///< max unsigned 16bit value
#    define s16_min     (-0x7fff - 1)  ///< min signed 16bit value
#    define s16_max     0x7fff         ///< max signed 16bit value

/**
 * @typedef u32
 * @short unsigned 32bit int type
 *
 * @typedef s32
 * @short signed 32bit int type
 */
#    if __SIZEOF_INT__ == 4
typedef unsigned int u32;
typedef signed int s32;
#    elif __SIZEOF_LONG__ == 4
typedef unsigned long u32;
typedef signed long s32;
#    else
#        error Neither 'int' nor 'short' is 4 bytes long!
#    endif

#    define u32_min     0              ///< min unsigned 32bit value
#    define u32_max     0xffffffffU    ///< max unsigned 32bit value
#    define s32_min     (-0x7fffffff - 1)   ///< min signed 32bit value
#    define s32_max     0x7fffffff     ///< max signed 32bit value

/**
 * @typedef u64
 * @short unsigned 16bit int type
 *
 * @typedef s64
 * @short signed 16bit int type
 */
/** TODO: 32bit systems do not use 64bit integers (?) */
#    if __SIZEOF_LONG__ == 8
typedef unsigned long u64;
typedef signed long s64;
#    elif __SIZEOF_LONG_LONG__ == 8
typedef unsigned long long u64;
typedef signed long long s64;
#    else
#        warning Neither 'long' nor 'long long' is 8 bytes long!
#    endif

#    define u64_min 0                  ///< min unsigned 64bit value
#    define u64_max 0xffffffffffffffffU ///< max unsigned 64bit value
#    define s64_min (-0x7fffffffffffffff - 1)   ///< min signed 64bit value
#    define s64_max 0x7fffffffffffffff ///< max signed 64bit value

#    define FALSE   0                  ///< logical false value
#    define TRUE    1                  //< logical true value
#    define bool    char               //< bool type
#    define size_t  u32                //< size type
#    define ssize_t s32                //< signed size type
#    define off_t   s64                //< offset type

/**
 * @typedef addr_t
 * @short pointer address type
 *
 * @typedef register_t
 * @short register type
 *
 * @def p_max
 * @short pointer address max value
 *
 * @def __BITS__
 * @short max supported bit ops
 *
 * @def __signed
 * @short default signed int
 *
 * @def __unsigned
 * @short default unsigned int
 *
 * @def __unsigned_max
 * @short default unsigned int max value
 *
 * @def __signed_max
 * @short default signed int max value
 */
#    if __SIZEOF_POINTER__ == 1
typedef u8 addr_t;
typedef u8 register_t;
#        define p_max           u8_max
#        define __BITS__        8
#        define __signed        s8
#        define __unsigned      u8
#        define __unsigned_max  8_max
#        define __signed_max    s8_max
#    elif __SIZEOF_POINTER__ == 2
typedef u16 addr_t;
typedef u16 register_t;
#        define p_max           u16_max
#        define __BITS__        16
#        define __signed        s16
#        define __unsigned      u16
#        define __unsigned_max  u16_max
#        define __signed_max    s16_max
#    elif __SIZEOF_POINTER__ == 4
typedef u32 addr_t;
typedef u32 register_t;
#        define p_max           u32_max
#        define __BITS__        32
#        define __signed        s32
#        define __unsigned      u32
#        define __unsigned_max  u32_max
#        define __signed_max    s32_max
#    elif __SIZEOF_POINTER__ == 8
typedef u64 addr_t;
typedef u64 register_t;
#        define p_max           u64_max
#        define __BITS__        64
#        define __signed        s64
#        define __unsigned      u64
#        define __unsigned_max  u64_max
#        define __signed_max    s64_max
#    else
#        error Unknown pointer size!
#    endif

typedef addr_t ptrdiff_t;

#endif /* !__flos__TYPES_H__ */
