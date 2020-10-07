/*
 * Copyright (C) 2018 Koen Zandberg <koen@bergzand.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       Test application for the xpt2046 touch screen sensor
 *
 * @author      Koen Zandberg <koen@bergzand.net>
 *
 * @}
 */

#ifndef TEST_SPI
#error "TEST_SPI not defined"
#endif

#ifndef TEST_SPI_CS
#error "TEST_SPI_CS not defined"
#endif

#ifndef TEST_SPI_CLK
#error "TEST_SPI_CLK not defined"
#endif

#include <stdio.h>
#include "xtimer.h"
#include "xpt2046.h"
#include "periph/spi.h"

int main(void)
{
    xpt2046_t dev;
    xpt2046_params_t params = {
        .spi = TEST_SPI,
        .spi_clk = TEST_SPI_CLK,
        .cs_pin = TEST_SPI_CS,
    };

    puts("xpt2046 touch screen test application\n");

    /* initialize the sensor */
    printf("Initializing touch screen...");

    if (xpt2046_init(&dev, &params) == 0) {
        puts("[OK]");
    }
    else {
        puts("[Failed]");
        return 1;
    }

    xtimer_ticks32_t ticks = xtimer_now();

    while (1) {
        xpt2046_xyz_t m;
        xpt2046_get_xyz(&dev, &m);
        printf("X: %" PRIu16 ", Y: %" PRIu16 ", Z: %" PRIu16"\n", m.x, m.y, m.z);
        xtimer_periodic_wakeup(&ticks, 200 * US_PER_MS);
    }

    return 0;
}
