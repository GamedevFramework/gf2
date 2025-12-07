#!/bin/sh

SOURCES="default.vert default.frag text.frag fullscreen.vert fullscreen.frag"

for SOURCE in ${SOURCES}
do
  BINARY="${SOURCE}.bin"
  glslc -c --target-env=vulkan1.0 -mfmt=bin "${SOURCE}" -o "${BINARY}"

  HEADER="${SOURCE}.h"
  hexdump -ve '1/1 "0x%.2x, "' "${BINARY}" > "${HEADER}"
  echo >> "${HEADER}"

  rm -f ${BINARY}
done
