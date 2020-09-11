BOARD ?= stm32f429i-disc1
TEST_OUTPUT ?= 1
include ../Makefile.tests_common

# No interactive_sync
DISABLE_MODULE += test_utils_interactive_sync

USEPKG += lvgl
USEMODULE += lvgl_contrib
ifeq ($(TEST_OUTPUT),1)
  USEMODULE += ili9341
endif
ifeq ($(TEST_OUTPUT),2)
  USEMODULE += lvgl_sdl
  USEMODULE += lvgl_contrib_touch
  CFLAGS += -DUSE_LVGL_SDL -DMODULE_TOUCH_DEV
endif


include $(RIOTBASE)/Makefile.include
