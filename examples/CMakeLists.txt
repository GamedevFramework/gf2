set(GF_EXAMPLE_ASSETS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

# 00-triangle

add_executable(00-triangle
  00-triangle.cc
)

target_link_libraries(00-triangle
  PRIVATE
    gf2graphics0
)

# 01-rectangle

add_executable(01-rectangle
  01-rectangle.cc
)

target_link_libraries(01-rectangle
  PRIVATE
    gf2graphics0
)

# 02-texture

add_executable(02-texture
  02-texture.cc
)

target_link_libraries(02-texture
  PRIVATE
    gf2graphics0
)

target_include_directories(02-texture
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 03-camera

add_executable(03-camera
  03-camera.cc
)

target_link_libraries(03-camera
  PRIVATE
    gf2graphics0
)

target_include_directories(03-camera
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 04-render-texture

add_executable(04-render-texture
  04-render-texture.cc
)

target_link_libraries(04-render-texture
  PRIVATE
    gf2graphics0
)

target_include_directories(04-render-texture
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 05-sprite

add_executable(05-sprite
  05-sprite.cc
)

target_link_libraries(05-sprite
  PRIVATE
    gf2graphics0
)

target_include_directories(05-sprite
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 06-text

add_executable(06-text
  06-text.cc
)

target_link_libraries(06-text
  PRIVATE
    gf2graphics0
)

target_include_directories(06-text
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 07-animation

add_executable(07-animation
  07-animation.cc
)

target_link_libraries(07-animation
  PRIVATE
    gf2graphics0
)

target_include_directories(07-animation
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

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



# 13-shape

add_executable(13-shape
  13-shape.cc
)

target_link_libraries(13-shape
  PRIVATE
    gf2graphics0
)
