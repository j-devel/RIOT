PKG_NAME=lvgl
PKG_URL=https://github.com/littlevgl/lvgl
PKG_VERSION=f6f5f691f73e433eb32f831c8356e602058267fb
PKG_LICENSE=MIT

include $(RIOTBASE)/pkg/pkg.mk

TDIR = $(RIOTPKG)/$(PKG_NAME)
PDIR = $(PKG_BUILDDIR)

.PHONY: all

SUBMODS := $(filter lvgl_%,$(USEMODULE))

all: $(SUBMODS)

# lvgl modules
lvgl_core:
	"$(MAKE)" -C $(PDIR)/src/lv_core -f $(TDIR)/lvgl.core.mk

lvgl_draw:
	"$(MAKE)" -C $(PDIR)/src/lv_draw -f $(TDIR)/lvgl.draw.mk

lvgl_misc:
	"$(MAKE)" -C $(PDIR)/src/lv_misc -f $(TDIR)/lvgl.misc.mk

lvgl_objx:
	"$(MAKE)" -C $(PDIR)/src/lv_objx -f $(TDIR)/lvgl.objx.mk

lvgl_themes:
	"$(MAKE)" -C $(PDIR)/src/lv_themes -f $(TDIR)/lvgl.themes.mk

lvgl_font:
	"$(MAKE)" -C $(PDIR)/src/lv_font -f $(TDIR)/lvgl.font.mk

lvgl_hal:
	"$(MAKE)" -C $(PDIR)/src/lv_hal -f $(TDIR)/lvgl.hal.mk

lvgl_sdl:
	"$(MAKE)" -C $(TDIR)/lv_sdl -f $(TDIR)/lvgl.sdl.mk
