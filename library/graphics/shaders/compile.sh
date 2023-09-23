#!/bin/sh

#
# For now, use this script to generate shader code in text form.
# When support will be available in xmake (https://github.com/xmake-io/xmake/issues/4099),
# we will do it in build files.
#

glslc -c --target-env=vulkan1.2 -mfmt=num simple.vert -o simple.vert.h
glslc -c --target-env=vulkan1.2 -mfmt=num simple.frag -o simple.frag.h

glslc -c --target-env=vulkan1.2 -mfmt=num default.vert -o default.vert.h
glslc -c --target-env=vulkan1.2 -mfmt=num default.frag -o default.frag.h

glslc -c --target-env=vulkan1.2 -mfmt=num text.frag -o text.frag.h

glslc -c --target-env=vulkan1.2 -mfmt=num fullscreen.vert -o fullscreen.vert.h
glslc -c --target-env=vulkan1.2 -mfmt=num fullscreen.frag -o fullscreen.frag.h
