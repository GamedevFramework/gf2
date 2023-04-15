set_project("gf")
set_version("0.0.1")

add_requires("sdl2", "fmt")

add_rules("mode.asan", "mode.tsan", "mode.ubsan", "mode.coverage", "mode.debug", "mode.releasedbg", "mode.release")

set_warnings("all")
set_languages("cxx17")

if is_plat("windows") then
  add_cxflags("/wd4251") -- Disable warning: class needs to have dll-interface to be used by clients of class blah blah blah
end

target("gf2core0")
    set_kind("shared")
    add_defines("GF_CORE_BUILD")
    add_files("library/core/*.cc")
    add_includedirs("include", { public = true })
    add_packages("fmt", { public = true })

target("gf2graphics0")
    set_kind("shared")
    add_defines("GF_GRAPHICS_BUILD")
    add_files("library/graphics/*.cc")
    add_includedirs("include", { public = true })
    add_packages("sdl2")
    add_deps("gf2core0")

target("gf2_core_tests")
    set_kind("binary")
    set_languages("cxx17")
    add_files("tests/tests_core_*.cc", "tests/main.cc")
    add_files("tests/googletest/googletest/src/gtest-all.cc")
    add_includedirs("tests/googletest/googletest")
    add_includedirs("tests/googletest/googletest/include")
    add_deps("gf2core0")
    if is_plat("linux") then
        add_syslinks("pthread")
    end
