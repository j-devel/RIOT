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
 * @brief       Device driver implementation for the XPT2046 touch screen sensors
 *
 * @author      Koen Zandberg <koen@bergzand.net>
 *
 * @}
 */


#include <string.h>
#include "periph/spi.h"
#include "xpt2046.h"
#include "xpt2046_internal.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

const uint8_t measure_xyz[] = {
    XPT2046_START | XPT2046_Z1 | XPT2046_ADC_ON,
    0x00,
    XPT2046_START | XPT2046_Z2 | XPT2046_ADC_ON,
    0x00,
    XPT2046_START | XPT2046_Y | XPT2046_ADC_ON,
    0x00,
    XPT2046_START | XPT2046_X | XPT2046_OFF,
    0x00,
    0x00,
};

const uint8_t measure_temp[] = {
    XPT2046_START | XPT2046_TEMP0 | XPT2046_SER | XPT2046_ALL_ON,
    0x00,
    XPT2046_START | XPT2046_TEMP1 | XPT2046_SER | XPT2046_ALL_ON,
    0x00,
    XPT2046_START | XPT2046_SER | XPT2046_ALL_ON,
    0x00,
    0x00,
};

static uint16_t diff_to_kelvin(uint16_t adc_diff)
{
    return (1472) * adc_diff;
}

int xpt2046_get_xyz(const xpt2046_t *dev, xpt2046_xyz_t *measure)
{
    (void)measure;
    uint8_t recv[sizeof(measure_xyz)];
    spi_acquire(dev->params->spi, dev->params->cs_pin, SPI_MODE_0,
                dev->params->spi_clk);
    spi_transfer_bytes(dev->params->spi, dev->params->cs_pin, false,
                       measure_xyz, recv, sizeof(measure_xyz));
    spi_release(dev->params->spi);
    measure->y = ((recv[5] << 5) | (recv[6] >> 3));
    measure->x = ((recv[7] << 5) | (recv[8] >> 3));
    uint16_t z1 = ((recv[1] << 5) | (recv[2] >> 3));
    uint16_t z2 = ((recv[3] << 5) | (recv[4] >> 3));
    measure->z = ((((uint32_t)z2 * (uint32_t)measure->x) / z1) - measure->x);
    return measure->z < XPT2046_PRESSURE_TOUCH_LEVEL ?
            XPT2046_TOUCH_DETECTED : XPT2046_NO_TOUCH;
}

uint16_t xpt2046_get_temp(const xpt2046_t *dev)
{
    uint8_t recv[sizeof(measure_temp)];

    spi_acquire(dev->params->spi, dev->params->cs_pin, SPI_MODE_0,
                dev->params->spi_clk);
    spi_transfer_bytes(dev->params->spi, dev->params->cs_pin, false,
                       measure_temp, recv, sizeof(measure_xyz));
    spi_release(dev->params->spi);
    uint16_t temp0 = ((recv[3] * 32) | (recv[4] >> 3));
    uint16_t temp1 = ((recv[5] * 32) | (recv[6] >> 3));
    uint16_t kelvin = diff_to_kelvin(temp1 - temp0);
    DEBUG("[xpt2046]: recv: %x, %x, %x, %x, %x, %x, %x\n", recv[0], recv[1],
          recv[2], recv[3], recv[4], recv[5], recv[6]);
    DEBUG("[xpt2046]: temperature: %d K, t0: %d, t1: %d\n", kelvin, temp0,
          temp1);
    return kelvin;
}

int xpt2046_init(xpt2046_t *dev, const xpt2046_params_t *params)
{
    dev->params = params;
    int res = spi_init_cs(dev->params->spi, dev->params->cs_pin);
    if (res != SPI_OK) {
        DEBUG("[xpt2046] init: error initializing the CS pin [%i]\n", res);
        return XPT2046_ERR_SPI;
    }
    return XPT2046_OK;
}
