find_package(Threads REQUIRED)

include(CTest)
include(GoogleTest)

# gf::core tests

add_executable(gf2_core_tests
  main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest-all.cc
  tests_core_Color.cc
  tests_core_Flags.cc
  tests_core_Id.cc
  tests_core_Math.cc
  tests_core_Range.cc
  tests_core_Rect.cc
  tests_core_Ref.cc
  tests_core_Resource.cc
  tests_core_Span.cc
  tests_core_Vec2.cc
  tests_core_Vec3.cc
)

target_include_directories(gf2_core_tests
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest
)

target_link_libraries(gf2_core_tests
  PRIVATE
    gf2core0
    Threads::Threads
)

gtest_discover_tests(gf2_core_tests)
