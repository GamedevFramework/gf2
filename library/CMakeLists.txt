
function(gf2_handle_library target)
  set(options "")
  set(value_options EXPORT_NAME EXPORT_MACRO_NAME EXPORT_FILE_NAME)
  set(multi_value_options "")

  cmake_parse_arguments(MY "${options}" "${value_options}" "${multi_value_options}" ${ARGN})

  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${target}
      PUBLIC GF_STATIC
    )

    set_target_properties(${target}
      PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )
  endif()

  if(GF2_DEBUG)
    target_compile_definitions(${target}
      PRIVATE GF_DEBUG
    )
  endif()

  target_compile_features(${target}
    PUBLIC
      cxx_std_17
  )

  set_target_properties(${target}
    PROPERTIES
      EXPORT_NAME "${MY_EXPORT_NAME}"
      CXX_STANDARD 17
      CXX_EXTENSIONS OFF
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN ON
      VERSION ${PROJECT_VERSION}
      SOVERSION ${PROJECT_VERSION_MAJOR}
  )

  generate_export_header(${target}
    BASE_NAME "${MY_EXPORT_NAME}"
    EXPORT_MACRO_NAME "${MY_EXPORT_MACRO_NAME}"
    EXPORT_FILE_NAME "${MY_EXPORT_FILE_NAME}"
    STATIC_DEFINE STATIC
    PREFIX_NAME GF_
  )

endfunction()

# gf2::core

add_library(gf2core0
  core/Array2D.cc
  core/Color.cc
  core/Flags.cc
  core/Math.cc
  core/Mat3.cc
  core/Range.cc
  core/Rect.cc
  core/Transform.cc
  core/Vec2.cc
  core/Vec3.cc
)

gf2_handle_library(gf2core0
  EXPORT_NAME
    "core"
  EXPORT_MACRO_NAME
    CORE_API
  EXPORT_FILE_NAME
    "CoreApi.h"
)

# gf2::graphics

add_library(gf2graphics0
  graphics/Keyboard.cc
  graphics/Mouse.cc
)

gf2_handle_library(gf2graphics0
  EXPORT_NAME
    "graphics"
  EXPORT_MACRO_NAME
    GRAPHICS_API
  EXPORT_FILE_NAME
    "GraphicsApi.h"
)

target_link_libraries(gf2graphics0
  PUBLIC
    gf2core0
  PRIVATE
    SDL2::SDL2
)
