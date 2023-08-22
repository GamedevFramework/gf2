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
