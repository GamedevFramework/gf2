#!/bin/sh

SOURCES="default.vert default.frag text.frag fullscreen.vert fullscreen.frag"

for SOURCE in ${SOURCES}
do
  BINARY="${SOURCE}.bin"
  glslc -c --target-env=vulkan1.0 -mfmt=bin "${SOURCE}" -o "${BINARY}"

  HEADER_SPIRV="${SOURCE}.spirv.h"
  hexdump -ve '1/1 "0x%.2x, "' "${BINARY}" > "${HEADER_SPIRV}"
  echo >> "${HEADER_SPIRV}"

  TEXT_MSL="${SOURCE}.msl"
  HEADER_MSL="${SOURCE}.msl.h"
  spirv-cross --msl --msl-decoration-binding "${BINARY}" > "${TEXT_MSL}"
  hexdump -ve '1/1 "0x%.2x, "' "${TEXT_MSL}" > "${HEADER_MSL}"
  echo >> "${HEADER_MSL}"

#   rm -f ${BINARY} ${TEXT_MSL}
done
