cmake_minimum_required(VERSION 3.10)

project(GF
  VERSION 1.1.0
  LANGUAGES CXX C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build the library as a shared library" ON)
option(BUILD_TESTING "Build tests" ON)

option(GF_USE_EMBEDDED_LIBS "Build the library with the submodule libraries" ON)

set(SDL2_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED)
find_package(fmt REQUIRED)
find_package(ZLIB REQUIRED)

if(GF_USE_EMBEDDED_LIBS)
  message(STATUS "Build with embedded libraries")
  set(Stb_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/library/vendor/stb")
else()
  # works on vcpkg
  find_package(Stb REQUIRED)
endif()

if(MSVC)
  message(STATUS "Using MSVC compiler")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX /W3 /utf-8 /wd4251")
else()
  message(STATUS "Using GCC or Clang compiler")
  # assume it's a GNU-like compiler (GCC, clang)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

add_subdirectory(library)

add_subdirectory(docs/snippets)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

