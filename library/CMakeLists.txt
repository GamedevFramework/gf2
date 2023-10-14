
function(gf2_handle_library target)
  set(options "")
  set(value_options EXPORT_NAME)
  set(multi_value_options "")

  cmake_parse_arguments(MY "${options}" "${value_options}" "${multi_value_options}" ${ARGN})

  string(TOUPPER "${MY_EXPORT_NAME}" LIBRARY_NAME)

  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${target}
      PUBLIC
        GF_${LIBRARY_NAME}_STATIC
    )

    set_target_properties(${target}
      PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )
  else()
    target_compile_definitions(${target}
      PRIVATE
        GF_${LIBRARY_NAME}_BUILD
    )
  endif()

  if(GF2_DEBUG)
    target_compile_definitions(${target}
      PRIVATE GF_DEBUG
    )
  endif()

  target_compile_features(${target}
    PUBLIC
      cxx_std_17
  )

  if(MSVC)
    target_compile_definitions(${target}
      PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )
  endif()

  set_target_properties(${target}
    PROPERTIES
      EXPORT_NAME "${MY_EXPORT_NAME}"
      CXX_STANDARD 17
      CXX_EXTENSIONS OFF
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN ON
      VERSION ${PROJECT_VERSION}
      SOVERSION ${PROJECT_VERSION_MAJOR}
  )
endfunction()

# gf2::core

add_library(gf2core0
  core/AnimationData.cc
  core/AnyGrid.cc
  core/Array2D.cc
  core/BinaryHeap.cc
  core/BinPack.cc
  core/Bitmap.cc
  core/Blit.cc
  core/Camera.cc
  core/Circ.cc
  core/Clock.cc
  core/Color.cc
  core/Direction.cc
  core/Fixed.cc
  core/Flags.cc
  core/Font.cc
  core/FontManager.cc
  core/GridMap.cc
  core/Grids.cc
  core/Id.cc
  core/Image.cc
  core/Log.cc
  core/Math.cc
  core/Mat3.cc
  core/Mat4.cc
  core/Move.cc
  core/Orientation.cc
  core/Polygon.cc
  core/Property.cc
  core/PropertyMap.cc
  core/Random.cc
  core/Range.cc
  core/Rect.cc
  core/ResourceBundle.cc
  core/ResourceContext.cc
  core/ResourceLoader.cc
  core/ResourceLoaders.cc
  core/ResourceManager.cc
  core/ResourceRegistry.cc
  core/Result.cc
  core/Serialization.cc
  core/SerializationOps.cc
  core/ShapeData.cc
  core/SpriteData.cc
  core/Stream.cc
  core/Streams.cc
  core/StringUtils.cc
  core/Tarball.cc
  core/TextData.cc
  core/TiledMapData.cc
  core/Time.cc
  core/Transform.cc
  core/Vec2.cc
  core/Vec3.cc
)

gf2_handle_library(gf2core0
  EXPORT_NAME
    "core"
)

target_link_libraries(gf2core0
  PUBLIC
    fmt::fmt-header-only
    ZLIB::ZLIB
  PRIVATE
    Freetype::Freetype
    pugixml
)

target_compile_definitions(gf2core0
  PRIVATE
    ZLIB_CONST
)

target_include_directories(gf2core0
  SYSTEM
  PRIVATE
    "${Stb_INCLUDE_DIR}"
)

# gf2::graphics

add_library(gf2graphics0
  graphics/AnimatedSprite.cc
  graphics/Buffer.cc
  graphics/CommandBuffer.cc
  graphics/Descriptor.cc
  graphics/Entity.cc
  graphics/EntityContainer.cc
  graphics/Event.cc
  graphics/FontAtlas.cc
  graphics/Format.cc
  graphics/Gamepad.cc
  graphics/GraphicsInitializer.cc
  graphics/Keyboard.cc
  graphics/Mouse.cc
  graphics/Renderer.cc
  graphics/RenderPipeline.cc
  graphics/RenderRecorder.cc
  graphics/RenderTarget.cc
  graphics/Scene.cc
  graphics/SceneManager.cc
  graphics/Shader.cc
  graphics/ShaderData.cc
  graphics/Shape.cc
  graphics/ShapeEntity.cc
  graphics/Sprite.cc
  graphics/SpriteEntity.cc
  graphics/Text.cc
  graphics/TextEntity.cc
  graphics/Texture.cc
  graphics/TiledMap.cc
  graphics/Touch.cc
  graphics/Vertex.cc
  graphics/VertexInput.cc
  graphics/Vulkan.cc
  graphics/Window.cc
)

gf2_handle_library(gf2graphics0
  EXPORT_NAME
    "graphics"
)

target_link_libraries(gf2graphics0
  PUBLIC
    gf2core0
    Vulkan::Headers
    volk::volk_headers
    GPUOpen::VulkanMemoryAllocator
  PRIVATE
    SDL2::SDL2
)

target_compile_definitions(gf2graphics0
  PUBLIC
    VK_NO_PROTOTYPES
)

if(GF_USE_EMBEDDED_LIBS)
  target_sources(gf2graphics0
    PRIVATE
      "vendor/vk-bootstrap/src/VkBootstrap.cpp"
  )

  target_include_directories(gf2graphics0
    SYSTEM
    PRIVATE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/vk-bootstrap/src>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
else()
  target_link_libraries(gf2graphics0
    PRIVATE
      vk-bootstrap::vk-bootstrap
  )
endif()


target_include_directories(gf2graphics0
  PRIVATE
    "graphics/shaders"
)

# add_subdirectory(graphics/shaders)

# gf2::network

add_library(gf2network0
  network/NetworkInitializer.cc
  network/Packet.cc
  network/Queue.cc
  network/Socket.cc
  network/SocketAddress.cc
  network/SocketSelector.cc
  network/SocketTypes.cc
  network/TcpListener.cc
  network/TcpSocket.cc
  network/UdpSocket.cc
  network/bits/SocketPortability.cc
)

gf2_handle_library(gf2network0
  EXPORT_NAME
    "network"
)

target_link_libraries(gf2network0
  PUBLIC
    gf2core0
)

if (WIN32)
  target_link_libraries(gf2network0
    PUBLIC
      ws2_32
  )
endif()

if (UNIX AND NOT APPLE)
  target_compile_definitions(gf2network0
    PUBLIC
      _POSIX_C_SOURCE=200809L
  )
endif()

# gf2::audio

add_library(gf2audio0
  audio/AudioListener.cc
  audio/AudioManager.cc
  audio/AudioSource.cc
  audio/AudioSpatialization.cc
  audio/Music.cc
  audio/Sound.cc
  audio/bits/AudioBackend.cc
)

set_source_files_properties(
  audio/bits/AudioBackend.cc
  PROPERTIES
    SKIP_UNITY_BUILD_INCLUSION ON
)

gf2_handle_library(gf2audio0
  EXPORT_NAME
    "audio"
)

target_link_libraries(gf2audio0
  PUBLIC
    gf2core0
  PRIVATE
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

target_include_directories(gf2audio0
  SYSTEM
  PRIVATE
    "${MINIAUDIO_INCLUDE_DIR}"
    "${Stb_INCLUDE_DIR}"
)

# install

install(
  TARGETS gf2core0 gf2graphics0 gf2network0 gf2audio0
  EXPORT gf2Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(GF_USE_EMBEDDED_LIBS)
  install(
    FILES
      vendor/vk-bootstrap/src/VkBootstrap.h
      vendor/vk-bootstrap/src/VkBootstrapDispatch.h
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  )
endif()

install(
  EXPORT gf2Targets
  NAMESPACE gf2::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gf2"
)

install(
  FILES gf2Config.cmake
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gf2"
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/gf2ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/gf2ConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gf2"
)
