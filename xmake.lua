set_project("gf")
set_version("0.1.0")

add_requires("libsdl", "freetype", "zlib")
add_requires("fmt", { system = false, configs = { header_only = true }})
add_requires("glslang", { configs = { binaryonly = true }})
add_requires("miniaudio 0.11.17")
add_requires("stb", { system = false })
add_requires("vk-bootstrap")
add_requires("volk", { configs = { header_only = true }})
add_requires("vulkan-headers")
-- add_requires("vulkan-validationlayers")
add_requires("vulkan-memory-allocator")

add_rules("mode.asan", "mode.tsan", "mode.ubsan", "mode.coverage", "mode.debug", "mode.releasedbg", "mode.release")

set_policy("build.warning", true)
set_warnings("allextra")
set_languages("cxx17")

if is_plat("windows") then
  add_cxflags("/wd4251") -- Disable warning: class needs to have dll-interface to be used by clients of class blah blah blah
  add_cxflags("/utf-8")
  add_defines("NOMINMAX", "_CRT_SECURE_NO_WARNINGS")
end

target("gf2core0")
    set_kind("shared")
    add_defines("GF_CORE_BUILD")
    add_defines("ZLIB_CONST")
    add_files("library/core/*.cc")
    add_includedirs("include", { public = true })
    add_packages("freetype", "stb")
    add_packages("fmt", "zlib", { public = true })

target("gf2graphics0")
    set_kind("shared")
    add_defines("GF_GRAPHICS_BUILD")
    add_defines("VK_NO_PROTOTYPES", { public = true })
    add_files("library/graphics/*.cc")
    add_includedirs("include", { public = true })
    add_packages("libsdl", "volk", "vulkan-headers", "vulkan-memory-allocator", { public = true })
    add_packages("vk-bootstrap") -- , "vulkan-validationlayers", "glslang"
--     add_rules("utils.glsl2spv", { outputdir = "$(buildir)/shaders", bin2c = true })
--     add_files("library/graphics/shaders/*.vert", "library/graphics/shaders/*.frag")
    add_includedirs("library/graphics/shaders")
    add_deps("gf2core0")

target("gf2network0")
    set_kind("shared")
    add_defines("GF_NETWORK_BUILD")
    add_files("library/network/*.cc")
    add_files("library/network/bits/*.cc")
    add_includedirs("include", { public = true })
    add_deps("gf2core0")
    if is_plat("windows") then
        add_syslinks("ws2_32")
    end

target("gf2audio0")
    set_kind("shared")
    add_defines("GF_AUDIO_BUILD")
    add_files("library/audio/*.cc")
    add_files("library/audio/bits/*.cc", { warnings = "none" })
    add_includedirs("include", { public = true })
    add_packages("miniaudio", "stb")
    add_deps("gf2core0")
    if is_plat("linux") then
        add_syslinks("dl")
    end

target("gf2_core_tests")
    set_kind("binary")
    set_languages("cxx17")
    add_files("tests/tests_core_*.cc", "tests/main_core.cc")
    add_files("tests/googletest/googletest/src/gtest-all.cc", { warnings = "none" })
    add_includedirs("tests/googletest/googletest")
    add_includedirs("tests/googletest/googletest/include")
    add_deps("gf2core0")
    if is_plat("linux") then
        add_syslinks("pthread")
    end
    set_configdir("$(buildir)/config/core_tests")
    set_configvar("GF_TEST_ASSETS_DIRECTORY", "$(projectdir)/tests/assets")
    add_configfiles("tests/config.h.in", {pattern = "@(.-)@"})
    add_includedirs("$(buildir)/config/core_tests")

target("gf2_network_tests")
    set_kind("binary")
    set_languages("cxx17")
    add_files("tests/tests_network_*.cc", "tests/main_network.cc")
    add_files("tests/googletest/googletest/src/gtest-all.cc", { warnings = "none" })
    add_includedirs("tests/googletest/googletest")
    add_includedirs("tests/googletest/googletest/include")
    add_deps("gf2network0")
    if is_plat("linux") then
        add_syslinks("pthread")
    end

target("gf2_audio_tests")
    set_kind("binary")
    set_languages("cxx17")
    add_files("tests/tests_audio_*.cc", "tests/main_audio.cc")
    add_files("tests/googletest/googletest/src/gtest-all.cc", { warnings = "none" })
    add_includedirs("tests/googletest/googletest")
    add_includedirs("tests/googletest/googletest/include")
    add_deps("gf2audio0")
    if is_plat("linux") then
        add_syslinks("pthread")
    end
    set_configdir("$(buildir)/config/audio_tests")
    set_configvar("GF_TEST_ASSETS_DIRECTORY", "$(projectdir)/tests/assets")
    add_configfiles("tests/config.h.in", {pattern = "@(.-)@"})
    add_includedirs("$(buildir)/config/audio_tests")
