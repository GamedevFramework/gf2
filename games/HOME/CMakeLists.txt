set(GF_HOME_ASSETS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(HOME
  HOME.cc
  bits/GameHub.cc
  bits/HeroEntity.cc
  bits/MapEntity.cc
  bits/ResourceHub.cc
  bits/SplashData.cc
  bits/SplashScene.cc
  bits/WorldData.cc
  bits/WorldScene.cc
)

target_link_libraries(HOME
  PRIVATE
    gf2audio0
    gf2graphics0
    gf2physics0
)

target_include_directories(HOME
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)
