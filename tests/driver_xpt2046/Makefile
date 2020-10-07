include ../Makefile.tests_common

BOARD ?= nrf52dk

USEMODULE += xpt2046
USEMODULE += xtimer
USEMODULE += printf_float
USEMODULE += core_thread_flags

# set default device parameters in case they are undefined
TEST_SPI ?= SPI_DEV\(0\)
TEST_SPI_CS ?= GPIO_PIN\(0,28\)
TEST_SPI_CLK ?= SPI_CLK_1MHZ

# export parameters
CFLAGS += -DTEST_SPI=$(TEST_SPI)
CFLAGS += -DTEST_SPI_CS=$(TEST_SPI_CS)
CFLAGS += -DTEST_SPI_CLK=$(TEST_SPI_CLK)

DEVELHELP := 1

include $(RIOTBASE)/Makefile.include
