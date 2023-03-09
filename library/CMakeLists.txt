
function(gf2_handle_library target export_name)
  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )


  if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${target}
      PUBLIC GF2_STATIC
    )

    set_target_properties(${target}
      PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )
  endif()

  target_compile_features(${target}
    PUBLIC
      cxx_std_17
  )

  set_target_properties(${target}
    PROPERTIES
      EXPORT_NAME "${export_name}"
      CXX_STANDARD 17
      CXX_EXTENSIONS OFF
      VERSION ${PROJECT_VERSION}
      SOVERSION ${PROJECT_VERSION_MAJOR}
  )
endfunction()

# gf2::core

add_library(gf2core0
  core/Math.cc
  core/Vec2.cc
  core/Vec3.cc
)

gf2_handle_library(gf2core0 "core")
