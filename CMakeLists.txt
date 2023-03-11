cmake_minimum_required(VERSION 3.10)

project(GF
  VERSION 1.1.0
  LANGUAGES CXX C
)

include(GenerateExportHeader)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build the library as a shared library" ON)
option(BUILD_TESTING "Build tests" ON)


add_subdirectory(library)

add_subdirectory(docs/snippets)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

