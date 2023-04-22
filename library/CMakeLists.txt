
function(gf2_handle_library target)
  set(options "")
  set(value_options EXPORT_NAME)
  set(multi_value_options "")

  cmake_parse_arguments(MY "${options}" "${value_options}" "${multi_value_options}" ${ARGN})

  string(TOUPPER "${MY_EXPORT_NAME}" LIBRARY_NAME)

  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${target}
      PUBLIC
        GF_${LIBRARY_NAME}_STATIC
    )

    set_target_properties(${target}
      PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )
  else()
    target_compile_definitions(${target}
      PRIVATE
        GF_${LIBRARY_NAME}_BUILD
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

  if(MSVC)
    target_compile_definitions(${target}
      PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )
  endif()

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
endfunction()

# gf2::core

add_library(gf2core0
  core/Array2D.cc
  core/Circ.cc
  core/Clock.cc
  core/Color.cc
  core/Flags.cc
  core/Id.cc
  core/Log.cc
  core/Math.cc
  core/Mat3.cc
  core/Random.cc
  core/Range.cc
  core/Rect.cc
  core/Ref.cc
  core/Time.cc
  core/Transform.cc
  core/Vec2.cc
  core/Vec3.cc
)

gf2_handle_library(gf2core0
  EXPORT_NAME
    "core"
)

target_link_libraries(gf2core0
  PUBLIC
    fmt::fmt-header-only
)

# gf2::graphics

add_library(gf2graphics0
  graphics/Event.cc
  graphics/Gamepad.cc
  graphics/GraphicsInitializer.cc
  graphics/Initializer.cc
  graphics/Keyboard.cc
  graphics/Mouse.cc
  graphics/Touch.cc
)

gf2_handle_library(gf2graphics0
  EXPORT_NAME
    "graphics"
)

target_link_libraries(gf2graphics0
  PUBLIC
    gf2core0
  PRIVATE
    SDL2::SDL2
)
