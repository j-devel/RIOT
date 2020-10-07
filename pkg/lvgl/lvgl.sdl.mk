MODULE = lvgl_sdl
CFLAGS += `sdl2-config --cflags`

include ${RIOTBASE}/Makefile.base
