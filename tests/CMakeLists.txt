include(CTest)
include(GoogleTest)

set(GF_TEST_ASSETS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

# gf::core tests

add_executable(gf2_core_tests
  main_core.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest-all.cc
  tests_core_Color.cc
  tests_core_Flags.cc
  tests_core_Id.cc
  tests_core_Math.cc
  tests_core_Move.cc
  tests_core_Range.cc
  tests_core_Rect.cc
  tests_core_Ref.cc
  tests_core_Resource.cc
  tests_core_Result.cc
  tests_core_Serialization.cc
  tests_core_Span.cc
  tests_core_Vec2.cc
  tests_core_Vec3.cc
)

target_include_directories(gf2_core_tests
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(gf2_core_tests
  PRIVATE
    gf2core0
    Threads::Threads
)

gtest_discover_tests(gf2_core_tests)

# gf::network tests

add_executable(gf2_network_tests
  main_network.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest-all.cc
  tests_network_TcpSocket.cc
  tests_network_UdpSocket.cc
)

target_include_directories(gf2_network_tests
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest
)

target_link_libraries(gf2_network_tests
  PRIVATE
    gf2network0
    Threads::Threads
)

gtest_discover_tests(gf2_network_tests)

# gf::audio tests

add_executable(gf2_audio_tests
  main_audio.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest-all.cc
  tests_audio_AudioManager.cc
  tests_audio_Sound.cc
)

target_include_directories(gf2_audio_tests
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(gf2_audio_tests
  PRIVATE
    gf2audio0
    Threads::Threads
)

gtest_discover_tests(gf2_audio_tests)
