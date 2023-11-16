# Very simple FindChipmunk.cmake that should work in the following situations:
# - Linux
# - macos
#
# For Windows, you should use vcpkg and the provided targets.
#
# Distributed under the terms and conditions of the MIT license
# Copyright (c) 2020 Julien Bernard
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

set(CHIPMUNK_SEARCH_PATHS
  ￼~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /opt/local
  /opt
)

# Chipmunk library

find_library(CHIPMUNK_LIBRARY
  NAMES
    chipmunk
  HINTS
    $ENV{CHIPMUNKDIR}
  PATH_SUFFIXES
    lib
  PATHS
    ${CHIPMUNK_SEARCH_PATHS}
)

find_path(CHIPMUNK_INCLUDE_DIR chipmunk/chipmunk.h
  HINTS
    $ENV{CHIPMUNKDIR}
  PATH_SUFFIXES
    include
  PATHS
    ${CHIPMUNK_SEARCH_PATHS}
)

if(NOT CHIPMUNK_INCLUDE_DIR)
  message(FATAL_ERROR "Could not find Chipmunk include directory.")
else()
  message(STATUS "Chipmunk include directory: ${CHIPMUNK_INCLUDE_DIR}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args("Chipmunk" DEFAULT_MSG
  CHIPMUNK_LIBRARY
  CHIPMUNK_INCLUDE_DIR
)

mark_as_advanced(
  CHIPMUNK_LIBRARY
  CHIPMUNK_INCLUDE_DIR
)

if(NOT TARGET Chipmunk::Chipmunk)
  add_library(Chipmunk::Chipmunk UNKNOWN IMPORTED)
  set_target_properties(Chipmunk::Chipmunk
    PROPERTIES
      IMPORTED_LOCATION "${CHIPMUNK_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${CHIPMUNK_INCLUDE_DIR}"
  )
endif()
