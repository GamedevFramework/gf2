if (NOT gf2_FIND_COMPONENTS)
  set (gf2_FIND_COMPONENTS "graphics")
endif()

include (CMakeFindDependencyMacro)

set (GF2_COMPONENTS "${gf2_FIND_COMPONENTS}")
if (gf2_FIND_REQUIRED)
  set (ERROR_LEVEL "FATAL_ERROR")
else ()
  set (ERROR_LEVEL "STATUS")
endif ()

# Include all gf targets
get_filename_component (GF2_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
include ("${GF2_CMAKE_DIR}/gf2Targets.cmake")

# Update components list depending on internal dependencies
foreach (component ${gf2_FIND_COMPONENTS})
  if ((${component} STREQUAL "graphics") OR (${component} STREQUAL "net") OR (${component} STREQUAL "audio"))
    list (APPEND GF2_COMPONENTS "core")
  endif ()
endforeach ()
list(REMOVE_DUPLICATES GF2_COMPONENTS)

# Macro to load dependencies
macro (load_dependencies component)
  # For gf2::core
  if (${component} STREQUAL "core")
    find_dependency(Threads)
    find_dependency(fmt)
    if (NOT BUILD_SHARED_LIBS)
      find_dependency(ZLIB)
      find_dependency(Freetype)
    endif()

  # For gf2::graphics
  elseif (${component} STREQUAL "graphics")
    find_dependency(Threads)
    if (NOT BUILD_SHARED_LIBS)
      find_dependency(SDL2)
      find_dependency(Vulkan)
      find_dependency(VulkanMemoryAllocator)
    endif()

  # For gf2::network
  elseif (${component} STREQUAL "network")
    # No dependencies

  # For gf2::audio
  elseif (${component} STREQUAL "audio")
    # No dependencies

  else ()
    message(${ERROR_LEVEL} "'${component}' is unknown")
  endif ()
endmacro ()


# Add all component
set (GF2_LIBRARIES "")
foreach (component ${GF2_COMPONENTS})
  if (TARGET "gf2::${component}")
    set(GF2_FOUND TRUE)

    # Load dependencies
    load_dependencies(${component})

    # Add component
    list (APPEND GF2_LIBRARIES "gf2::${component}")
  else ()
    message(${ERROR_LEVEL} "'gf2::${component}' component not found")
  endif ()
endforeach ()

# Get gf2 version info
include ("${GF2_CMAKE_DIR}/gf2ConfigVersion.cmake")

if ((NOT gf2_FIND_REQUIRED) AND (NOT GF2_FOUND))
  message (${ERROR_LEVEL} "gf2 not found: skipped")
elseif ((gf2_FIND_REQUIRED) AND (NOT GF2_FOUND))
  message (${ERROR_LEVEL} "gf2 not found: error")
else ()
  message(STATUS "Found gf2 (version \"${PACKAGE_VERSION}\") in ${CMAKE_CURRENT_LIST_DIR} with components: ${GF2_COMPONENTS}")
endif ()
