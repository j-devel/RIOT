LittlevGL sample application
============================

This application shows a basic usage of LittlevGL with RIOT. It's simply an
adaption of one of the upstream examples: the
[sysmon example](https://github.com/littlevgl/lv_examples/tree/master/lv_apps/sysmon).

### Flashing the application

The application works without modification on the stm32f429i-disc1 board. To
build, flash and run the application for this board, just use:

```
make BOARD=stm32f429i-disc1 -C tests/pkg_lvgl flash
```

### Expected result

The application displays a basic system monitor with the CPU and memory
usage of the board.
Note that on the pinetime, due to a too small screen resolution, some
information is cropped.

### Building native application with SDL backend

To facilitate developing/debugging LVGL applications, the native build option
using SDL backend is available. Invoke the provided build script as:

```
./build-with-sdl2.sh
```
