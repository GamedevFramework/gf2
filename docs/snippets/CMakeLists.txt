add_library(gf2_core_doc
  core_color.cc
  core_id.cc
  core_math.cc
)

target_link_libraries(gf2_core_doc
  PRIVATE
    gf2core0
)

add_library(gf2_graphics_doc
  graphics_event.cc
)

target_link_libraries(gf2_graphics_doc
  PRIVATE
    gf2graphics0
)
