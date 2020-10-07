/*
 * Copyright (C) 2018 Koen Zandberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_xpt2046
 * @{
 *
 * @file
 * @brief       Internal constants for xpt2046 touch screen sensors.
 *
 * @author      Koen Zandberg <koen@bergzand.net>
 *
 * @}
 */


#ifndef XPT2046_INTERNAL_H
#define XPT2046_INTERNAL_H

#include "periph/spi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XPT2046_SER     0x04 /* Single ended measurement */
#define XPT2046_8BIT    0x08
#define XPT2046_START   0x80

#define XPT2046_TEMP0   0x00
#define XPT2046_Y       0x10
#define XPT2046_VBAT    0x20
#define XPT2046_Z1      0x30
#define XPT2046_Z2      0x40
#define XPT2046_X       0x50
#define XPT2046_AUX     0x60
#define XPT2046_TEMP1   0x70

#define XPT2046_OFF     0x00
#define XPT2046_ADC_ON  0x01
#define XPT2046_REF_ON  0x02
#define XPT2046_ALL_ON  0x03

#ifdef __cplusplus
}
#endif

#endif /* XPT2046_INTERNAL_H */
/** @} */
