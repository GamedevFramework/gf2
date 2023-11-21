set(GF_EXAMPLE_ASSETS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

# 10-sprite

add_executable(10-sprite
  10-sprite.cc
)

target_link_libraries(10-sprite
  PRIVATE
    gf2graphics0
)

target_include_directories(10-sprite
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 11-text

add_executable(11-text
  11-text.cc
)

target_link_libraries(11-text
  PRIVATE
    gf2graphics0
)

target_include_directories(11-text
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 12-animation

add_executable(12-animation
  12-animation.cc
)

target_link_libraries(12-animation
  PRIVATE
    gf2graphics0
)

target_include_directories(12-animation
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 13-shape

add_executable(13-shape
  13-shape.cc
)

target_link_libraries(13-shape
  PRIVATE
    gf2graphics0
)


# 15-imgui

add_executable(15-imgui
  15-imgui.cc
)

target_link_libraries(15-imgui
  PRIVATE
    gf2imgui0
)

target_include_directories(15-imgui
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 20-physics

add_executable(20-physics
  20-physics.cc
)

target_link_libraries(20-physics
  PRIVATE
    gf2physics0
    gf2graphics0
)
