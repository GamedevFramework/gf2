add_library(gf2_core_doc
  core_clock.cc
  core_color.cc
  core_id.cc
  core_math.cc
  core_ref.cc
  core_time.cc
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

add_library(gf2_network_doc
  network_socket_selector.cc
  network_tcp_listener.cc
  network_tcp_socket.cc
  network_udp_socket.cc
)

target_link_libraries(gf2_network_doc
  PRIVATE
    gf2network0
)
