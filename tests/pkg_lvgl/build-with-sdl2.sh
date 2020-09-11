#!/bin/sh

echo \`make clean\` first in case \`TEST_OUTPUT\` is changed

# Update PATH for `sdl2-config`
PATH=../../build/pkg/sdl-local/bin:$PATH \
  TEST_OUTPUT=2 BOARD=native make
