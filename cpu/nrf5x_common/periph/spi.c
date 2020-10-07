/*
 * Copyright (C) 2014-2016 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     cpu_nrf5x_common
 * @ingroup     drivers_periph_spi
 * @{
 *
 * @file
 * @brief       Low-level SPI driver implementation
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 * @author      Frank Holtz <frank-riot2015@holtznet.de>
 * @author      Jan Wagner <mail@jwagner.eu>
 *
 * @}
 */
#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "mutex.h"
#include "assert.h"
#include "periph/spi.h"
#include "periph/gpio.h"

#define SPI_CPU_FLASH_END  (CPU_FLASH_BASE + FLASHPAGE_NUMOF * FLASHPAGE_SIZE)

#define THREAD_FLAG_SPI_BUS     (1<<10)

/**
 * @brief   array holding one pre-initialized mutex for each SPI device
 */
static mutex_t locks[SPI_NUMOF];
static kernel_pid_t pid[SPI_NUMOF];

static uint8_t _mbuf[SPI_NUMOF][UINT8_MAX];

static void _setup_workaround_for_ftpan_58(spi_t bus);

#ifdef CFU_FAM_NRF51
static inline NRF_SPI_Type *dev(spi_t bus)
{
    return spi_config[bus].dev;
}
#else
static inline NRF_SPIM_Type *dev(spi_t bus)
{
    return (NRF_SPIM_Type *)spi_config[bus].dev;
}
#endif

void spi_init(spi_t bus)
{
    assert(bus < SPI_NUMOF);

    /* initialize mutex */
    mutex_init(&locks[bus]);
    /* initialize pins */
    spi_init_pins(bus);
    NVIC_EnableIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);
}

void spi_init_pins(spi_t bus)
{
    /* set pin direction */
    gpio_init(spi_config[bus].sclk, GPIO_OUT);
    gpio_init(spi_config[bus].mosi, GPIO_OUT);
    gpio_init(spi_config[bus].miso, GPIO_IN);
    /* select pins for the SPI device */
#ifdef CPU_FAM_NRF51
    SPI_SCKSEL  = spi_config[bus].sclk;
    SPI_MOSISEL = spi_config[bus].mosi;
    SPI_MISOSEL = spi_config[bus].miso;
#else
    dev(bus)->PSEL.SCK  = spi_config[bus].sclk;
    dev(bus)->PSEL.MOSI = spi_config[bus].mosi;
    dev(bus)->PSEL.MISO = spi_config[bus].miso;

    _setup_workaround_for_ftpan_58(bus);
#endif
}

int spi_acquire(spi_t bus, spi_cs_t cs, spi_mode_t mode, spi_clk_t clk)
{
    (void) cs;

    mutex_lock(&locks[bus]);
#ifdef CPU_FAM_NRF51
    /* power on the bus (NRF51 only) */
    dev(bus)->POWER = 1;
#endif
    /* configure bus */
    dev(bus)->CONFIG = mode;
    dev(bus)->FREQUENCY = clk;
    /* enable the bus */
#ifdef CPU_FAM_NRF51
    dev(bus)->ENABLE = 1;
#else
    dev(bus)->ENABLE = SPIM_ENABLE_ENABLE_Enabled;
#endif
    return SPI_OK;
}

void spi_release(spi_t bus)
{
    /* power off everything */
    dev(bus)->ENABLE = 0;
#ifdef CPU_FAM_NRF51
    dev(bus)->POWER = 0;
#endif
    mutex_unlock(&locks[bus]);
}

#ifndef CPU_FAM_NRF51
/**
 * @brief Work-around for transmitting 1 byte with SPIM.
 *
 * @param spim: The SPIM instance that is in use.
 * @param ppi_channel: An unused PPI channel that will be used by the workaround.
 * @param gpiote_channel: An unused GPIOTE channel that will be used by the workaround.
 *
 * @warning Must not be used when transmitting multiple bytes.
 * @warning After this workaround is used, the user must reset the PPI channel and the GPIOTE channel before attempting to transmit multiple bytes.
 */
static void _setup_workaround_for_ftpan_58(spi_t bus)
{
    // Create an event when SCK toggles.
    NRF_GPIOTE->CONFIG[bus + 7] = (
        GPIOTE_CONFIG_MODE_Event <<
        GPIOTE_CONFIG_MODE_Pos
        ) | (
        spi_config[bus].sclk <<
        GPIOTE_CONFIG_PSEL_Pos
        ) | (
        GPIOTE_CONFIG_POLARITY_Toggle <<
        GPIOTE_CONFIG_POLARITY_Pos
        );

    // Stop the spim instance when SCK toggles.
    NRF_PPI->CH[bus].EEP = (uint32_t)&NRF_GPIOTE->EVENTS_IN[bus + 7];
    NRF_PPI->CH[bus].TEP = (uint32_t)&dev(bus)->TASKS_STOP;
}

static void _enable_workaround(spi_t bus)
{
    NRF_PPI->CHENSET = 1U << bus;
    // The spim instance cannot be stopped mid-byte, so it will finish
    // transmitting the first byte and then stop. Effectively ensuring
    // that only 1 byte is transmitted.
}

static void _clear_workaround(spi_t bus)
{
    NRF_PPI->CHENCLR = 1U << bus;
}

static void _transfer(spi_t bus, const uint8_t *out_buf, uint8_t *in_buf, uint8_t transfer_len)
{
    uint8_t out_len = (out_buf) ? transfer_len : 0;
    uint8_t in_len = (in_buf) ? transfer_len : 0;
    const uint8_t *out_mbuf = out_buf;
    if ((out_buf < (uint8_t*)SPI_CPU_FLASH_END) && (out_len)) {
        memcpy(_mbuf[bus], out_buf, out_len);
        out_mbuf = _mbuf[bus];
    }
    dev(bus)->TXD.PTR = (uint32_t)out_mbuf;
    dev(bus)->RXD.PTR = (uint32_t)in_buf;

    dev(bus)->TXD.MAXCNT = out_len;
    dev(bus)->RXD.MAXCNT = in_len;

    dev(bus)->EVENTS_END = 0;

    dev(bus)->TASKS_START = 1;
}
#endif



void spi_transfer_bytes(spi_t bus, spi_cs_t cs, bool cont,
                        const void *out, void *in, size_t len)
{
    const uint8_t *out_buf = out;
    uint8_t *in_buf = in;

    assert(out_buf || in_buf);

    if (cs != SPI_CS_UNDEF) {
        gpio_clear((gpio_t)cs);
    }
#ifdef CPU_FAM_NRF51
    for (int i = 0; i < (int)len; i++) {
        uint8_t tmp = (out_buf) ? out_buf[i] : 0;

        dev(bus)->EVENTS_READY = 0;
        dev(bus)->TXD = (uint8_t)tmp;
        while (dev(bus)->EVENTS_READY != 1);
        tmp = (uint8_t)dev(bus)->RXD;

        if (in_buf) {
            in_buf[i] = tmp;
        }
    }
#else
    dev(bus)->RXD.LIST = 0;
    dev(bus)->TXD.LIST = 0;

    if (len == 1) {
        _enable_workaround(bus);
        _transfer(bus, out_buf, in_buf, len);
        while (dev(bus)->EVENTS_END != 1);
        _clear_workaround(bus);
    }
    else {
        // Enable IRQ
        dev(bus)->EVENTS_END = 0;
        pid[bus] = sched_active_pid;
        dev(bus)->INTENSET = SPIM_INTENSET_END_Msk;
        do {
            size_t transfer_len = len > UINT8_MAX ? UINT8_MAX : len;
            _transfer(bus, out_buf, in_buf, transfer_len);
            thread_flags_wait_one(THREAD_FLAG_SPI_BUS);
            out_buf += out_buf ? transfer_len : 0;
            in_buf += in_buf? transfer_len : 0;
            len -= transfer_len;
        } while (len);
        dev(bus)->INTENCLR = SPIM_INTENCLR_END_Msk;
    }
#endif

    if ((cs != SPI_CS_UNDEF) && (!cont)) {
        gpio_set((gpio_t)cs);
    }
}

#ifndef CPU_FAM_NRF51
/* TODO: rework to allow arbitrary SPI peripheral order */
void isr_spi0_twi0(void)
{
    thread_flags_set((thread_t*)thread_get(pid[0]), THREAD_FLAG_SPI_BUS);
    dev(0)->EVENTS_END = 0;
    cortexm_isr_end();
}
#endif
