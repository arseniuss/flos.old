/**
 * @file    drivers/cmos/cmos.c
 * @brief   CMOS device
 * @version 1.0
 * @date    10.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

/**
 *  CMOS device should be attached at /dev/hw/cmos
 *  Reading it should return:
 *  fmt: dd.mm.yyyy. hh:mm Ddd alarm hh:mm
 *  exm: 10.10.2010. 15:51 Sat alarm 10:10
 *
 *  TODO
 */
