add_library(gf2_core_doc
  core_color.cc
  core_math.cc
)

target_link_libraries(gf2_core_doc
  PRIVATE
    gf2core0
)
