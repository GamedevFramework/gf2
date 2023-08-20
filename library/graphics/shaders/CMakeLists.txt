function(gf2_compile_shader target)
  set(options "")
  set(value_options ENV FORMAT EXTENSION)
  set(multi_value_options SOURCES)

  cmake_parse_arguments(MY "${options}" "${value_options}" "${multi_value_options}" ${ARGN})

  foreach(source ${MY_SOURCES})
    message(STATUS "Compiling ${source}...")

    add_custom_target(${source}.${MY_EXTENSION}
      ALL
      DEPENDS ${source}
      COMMAND
        Vulkan::glslc
        $<$<BOOL:${MY_ENV}>:--target-env=${MY_ENV}>
        $<$<BOOL:${MY_FORMAT}>:-mfmt=${MY_FORMAT}>
        -o "${CMAKE_CURRENT_BINARY_DIR}/${source}.${MY_EXTENSION}"
        "${CMAKE_CURRENT_SOURCE_DIR}/${source}"
    )

    add_dependencies(${target} ${source}.${MY_EXTENSION})
  endforeach()
endfunction()

gf2_compile_shader(gf2graphics0
  ENV
    vulkan1.2
  FORMAT
    c
  EXTENSION
    h
  SOURCES
    simple.frag
    simple.vert
)
