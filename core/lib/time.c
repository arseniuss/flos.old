/**
 * @file    lib/time.c
 * @brief   Time functions
 * @version 1.0
 * @date    10.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/types.h>

s64 timestamp;

s64 secs_of_years(int years) {
    s64 days = 0;

    while(years > 1969) {
        days += 365;
        if(years % 4 == 0) {
            if(years % 100 == 0) {
                if(years % 400 == 0) {
                    days++;
                }
            } else {
                days++;
            }
        }
        years--;
    }

    return days * 86400;
}

s64 secs_of_month(int months, int year) {
    year += 2000;

    s64 days = 0;
    switch (months) {
        case 11:
            days += 30;
        case 10:
            days += 31;
        case 9:
            days += 30;
        case 8:
            days += 31;
        case 7:
            days += 31;
        case 6:
            days += 30;
        case 5:
            days += 31;
        case 4:
            days += 30;
        case 3:
            days += 31;
        case 2:
            days += 28;
            if((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0))) {
                days++;
            }
        case 1:
            days += 31;
        default:
            break;
    }

    return days * 86400;
}

s64 get_unix_time(int year, int month, int day, int hour, int min, int sec) {
    s64 time = secs_of_years(year - 1) + secs_of_month(month - 1, year) +
        (day - 1) * 86400 + (hour * 3600) + (min * 60) + sec;

    return time;
}
