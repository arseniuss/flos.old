/**
 * @file    include/flos/time.h
 * @brief   Time functions
 * @version 1.0
 * @date    10.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__TIME_H__
#    define __flos__TIME_H__

s64 get_unix_time(int year, int month, int day, int hour, int min, int sec);

extern s64 timestamp;

#endif
