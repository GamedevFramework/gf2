cmake_minimum_required(VERSION 3.10)

project(GF
  VERSION 1.1.0
  LANGUAGES CXX C
)

include(GenerateExportHeader)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTING "Build tests" ON)


add_subdirectory(library)

add_subdirectory(docs/snippets)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

