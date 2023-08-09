set_project("gf")
set_version("0.1.0")

add_requires("libsdl", "freetype", "zlib")
add_requires("fmt", { system = false, configs = { header_only = true }})
add_requires("miniaudio 0.11.17")
add_requires("stb", { system = false })

add_rules("mode.asan", "mode.tsan", "mode.ubsan", "mode.coverage", "mode.debug", "mode.releasedbg", "mode.release")

set_warnings("allextra")
set_languages("cxx17")

if is_plat("windows") then
  add_cxflags("/wd4251") -- Disable warning: class needs to have dll-interface to be used by clients of class blah blah blah
  add_defines("NOMINMAX")
end

target("gf2core0")
    set_kind("shared")
    add_defines("GF_CORE_BUILD")
    add_defines("ZLIB_CONST")
    add_files("library/core/*.cc")
    add_includedirs("include", { public = true })
    add_packages("stb", "freetype")
    add_packages("fmt", { public = true })
    add_packages("zlib", { public = true })

target("gf2graphics0")
    set_kind("shared")
    add_defines("GF_GRAPHICS_BUILD")
    add_files("library/graphics/*.cc")
    add_includedirs("include", { public = true })
    add_packages("libsdl")
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
    add_files("library/audio/bits/*.cc")
    add_includedirs("include", { public = true })
    add_packages("miniaudio")
    add_packages("stb")
    add_deps("gf2core0")
    if is_plat("linux") then
        add_syslinks("dl")
    end

target("gf2_core_tests")
    set_kind("binary")
    set_languages("cxx17")
    add_files("tests/tests_core_*.cc", "tests/main_core.cc")
    add_files("tests/googletest/googletest/src/gtest-all.cc")
    add_includedirs("tests/googletest/googletest")
    add_includedirs("tests/googletest/googletest/include")
    add_deps("gf2core0")
    if is_plat("linux") then
        add_syslinks("pthread")
    end
    set_configvar("GF_TEST_ASSETS_DIRECTORY", "$(projectdir)/tests/assets")
    add_configfiles("tests/config.h.in", {pattern = "@(.-)@"})
    add_includedirs("$(buildir)")

target("gf2_network_tests")
    set_kind("binary")
    set_languages("cxx17")
    add_files("tests/tests_network_*.cc", "tests/main_network.cc")
    add_files("tests/googletest/googletest/src/gtest-all.cc")
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
    add_files("tests/googletest/googletest/src/gtest-all.cc")
    add_includedirs("tests/googletest/googletest")
    add_includedirs("tests/googletest/googletest/include")
    add_deps("gf2audio0")
    if is_plat("linux") then
        add_syslinks("pthread")
    end
    set_configvar("GF_TEST_ASSETS_DIRECTORY", "$(projectdir)/tests/assets")
    add_configfiles("tests/config.h.in", {pattern = "@(.-)@"})
    add_includedirs("$(buildir)")
