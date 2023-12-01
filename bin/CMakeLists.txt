
# gf2_tarball_listfiles

add_executable(gf2_tarball_listfiles
  gf2_tarball_listfiles.cc
)

target_link_libraries(gf2_tarball_listfiles
  PRIVATE
    gf2core0
)

# gf2_glyph_display

add_executable(gf2_glyph_display
  gf2_glyph_display.cc
)

target_link_libraries(gf2_glyph_display
  PRIVATE
    gf2core0
)

# gf2_tmx_display

add_executable(gf2_tmx_display
  gf2_tmx_display.cc
)

target_link_libraries(gf2_tmx_display
  PRIVATE
    gf2graphics0
)

# gf2_play_sound

add_executable(gf2_play_sound
  gf2_play_sound.cc
)

target_link_libraries(gf2_play_sound
  PRIVATE
    gf2audio0
)
