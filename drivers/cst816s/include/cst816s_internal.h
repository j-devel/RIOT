/*
 * Copyright (C) 2020 Koen Zandberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef CST816S_INTERNAL_H
#define CST816S_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define CST816S_RESET_DURATION_LOW      (20 * US_PER_MS)
#define CST816S_RESET_DURATION_HIGH     (400 * US_PER_MS)

#define CST816S_REG_INT_CNT                     0x8F
#define CST816S_REG_FLOW_WORK_CNT               0x91
#define CST816S_REG_WORKMODE                    0x00
#define CST816S_REG_WORKMODE_FACTORY_VALUE      0x40
#define CST816S_REG_WORKMODE_WORK_VALUE         0x00
#define CST816S_REG_CHIP_ID                     0xA3
#define CST816S_REG_CHIP_ID2                    0x9F
#define CST816S_REG_POWER_MODE                  0xA5
#define CST816S_REG_FW_VER                      0xA6
#define CST816S_REG_VENDOR_ID                   0xA8
#define CST816S_REG_LCD_BUSY_NUM                0xAB
#define CST816S_REG_FACE_DEC_MODE_EN            0xB0
#define CST816S_REG_GLOVE_MODE_EN               0xC0
#define CST816S_REG_COVER_MODE_EN               0xC1
#define CST816S_REG_CHARGER_MODE_EN             0x8B
#define CST816S_REG_GESTURE_EN                  0xD0
#define CST816S_REG_GESTURE_OUTPUT_ADDRESS      0xD3
#define CST816S_REG_ESD_SATURATE                0xED

#define CST816S_REG_POWER_MODE_SLEEP_VALUE      0x03


#ifdef __cplusplus
}
#endif

#endif /* CST816_INTERNAL_H */
/** @} */
