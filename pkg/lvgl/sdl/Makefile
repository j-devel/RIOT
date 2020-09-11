PKG_NAME=sdl
PKG_URL=https://github.com/j-devel/SDL
PKG_VERSION=a225dc4b67882663a92d6b6eea6d09e4a8dff3c1 # v2.0.12
PKG_LICENSE=https://github.com/j-devel/SDL/blob/master/COPYING.txt

include $(RIOTBASE)/pkg/pkg.mk

MODULE = lvgl_sdl

SDL_LOCAL = $(BUILD_DIR)/pkg/sdl-local

SDL_FLAGS = CFLAGS="-m32" LDFLAGS="-m32"
ifeq ($(shell uname -s),Darwin)
  SDL_FLAGS = CFLAGS="-arch i386" LDFLAGS="-arch i386"
endif

# Ubuntu:
# sudo apt-get install libx11-dev:i386
# Make sure x11 in driver support after `configure`:
#   Video drivers   : dummy x11(dynamic) opengl opengl_es2 vulkan

SDL_CONFIGURE = $(SDL_FLAGS) ./configure --prefix=$(SDL_LOCAL)
SDL_BUILD = mkdir -p $(SDL_LOCAL) && \
	    cd $(PKG_SOURCE_DIR) && $(SDL_CONFIGURE) && make && make install

all:
	@echo building 32bit SDL2
	if [ -d "$(SDL_LOCAL)" ]; then echo found $(SDL_LOCAL); else $(SDL_BUILD); fi

include $(RIOTBASE)/Makefile.base
