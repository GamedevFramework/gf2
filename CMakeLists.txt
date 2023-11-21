cmake_minimum_required(VERSION 3.10)

project(GF
  VERSION 0.1.0
  LANGUAGES CXX C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build the library as a shared library" ON)
option(BUILD_TESTING "Build tests" ON)

option(GF_USE_EMBEDDED_LIBS "Build the library with the submodule libraries" ON)

if(UNIX)
  option(GF_DEV_ENABLE_ASAN "Enable Address Sanitizer" OFF)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)

set(SDL2_BUILDING_LIBRARY TRUE)

find_package(Chipmunk REQUIRED)
find_package(fmt REQUIRED)
find_package(Freetype REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)
find_package(volk REQUIRED)
find_package(Vulkan REQUIRED COMPONENTS glslc)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

if(GF_USE_EMBEDDED_LIBS)
  message(STATUS "Build with embedded libraries")
  set(Stb_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/library/vendor/stb")
  set(MINIAUDIO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/library/vendor/miniaudio")
else()
  # works on vcpkg
  find_package(Stb REQUIRED)
  find_path(MINIAUDIO_INCLUDE_DIR "miniaudio.h")
  find_package(vk-bootstrap::vk-bootstrap REQUIRED)
endif()

if(MSVC)
  message(STATUS "Using MSVC compiler")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX /W4 /utf-8 /wd4251")
else()
  message(STATUS "Using GCC or Clang compiler")
  # assume it's a GNU-like compiler (GCC, clang)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -gdwarf-4")
endif()

if(UNIX)
  if(GF_DEV_ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif()
endif()

add_subdirectory(library)
add_subdirectory(docs/snippets)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

add_subdirectory(bin)
add_subdirectory(examples)
add_subdirectory(games/HOME)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/gf2"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
