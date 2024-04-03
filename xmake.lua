set_project("gf2")
set_version("0.1.0")

add_requires("libsdl", "freetype", "harfbuzz", "imgui", "pugixml", "zlib", "chipmunk2d")
add_requires("fmt", { system = false, configs = { header_only = true }})
add_requires("gtest", { configs = { gmock = false }})
add_requires("miniaudio 0.11.17")
add_requires("stb", { system = false })
add_requires("vk-bootstrap")
add_requires("volk", { configs = { header_only = true }})
add_requires("vulkan-headers")
-- add_requires("vulkan-validationlayers")
add_requires("vulkan-memory-allocator")

add_rules("mode.coverage", "mode.debug", "mode.releasedbg", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "$(buildir)"})

if is_mode("sanitizers") then
    set_symbols("debug")
    set_optimize("none")
    set_policy("build.sanitizer.address", true)
    set_policy("build.sanitizer.undefined", true)
end

set_policy("build.warning", true)
set_warnings("allextra")
set_languages("cxx17")
set_encodings("utf-8")
set_exceptions("cxx")

if is_plat("windows") then
  add_cxflags("/wd4251") -- Disable warning: class needs to have dll-interface to be used by clients of class blah blah blah
  add_defines("NOMINMAX", "_CRT_SECURE_NO_WARNINGS")
end

target("gf2core0")
    set_kind("shared")
    add_defines("GF_CORE_BUILD")
    add_defines("ZLIB_CONST")
    add_files("library/core/*.cc")
    add_headerfiles("include/core/*.h")
    add_includedirs("include", { public = true })
    add_packages("freetype", "pugixml", "stb")
    add_packages("fmt", "zlib", { public = true })

target("gf2graphics0")
    set_kind("shared")
    add_defines("GF_GRAPHICS_BUILD")
    add_defines("VK_NO_PROTOTYPES", { public = true })
    add_files("library/graphics/*.cc")
    add_headerfiles("include/graphics/*.h")
    add_includedirs("include", { public = true })
    add_packages("vulkan-headers", "vulkan-memory-allocator", { public = true })
    add_packages("harfbuzz", "libsdl", "volk", "vk-bootstrap") -- , "vulkan-validationlayers", "glslang"
--     add_rules("utils.glsl2spv", { outputdir = "$(buildir)/shaders", bin2c = true })
--     add_files("library/graphics/shaders/*.vert", "library/graphics/shaders/*.frag")
    add_includedirs("library/graphics/shaders")
    add_deps("gf2core0")

target("gf2network0")
    set_kind("shared")
    add_defines("GF_NETWORK_BUILD")
    add_files("library/network/*.cc")
    add_files("library/network/bits/*.cc")
    add_headerfiles("include/network/*.h")
    add_includedirs("include", { public = true })
    add_deps("gf2core0")
    if is_plat("windows") then
        add_syslinks("ws2_32")
    end

target("gf2audio0")
    set_kind("shared")
    add_defines("GF_AUDIO_BUILD")
    add_files("library/audio/*.cc")
    add_headerfiles("include/audio/*.h")
    add_files("library/audio/bits/*.cc", { warnings = "none" })
    add_includedirs("include", { public = true })
    add_packages("miniaudio", "stb")
    add_deps("gf2core0")
    if is_plat("linux") then
        add_syslinks("dl")
    end

target("gf2physics0")
    set_kind("shared")
    add_defines("GF_PHYSICS_BUILD")
    add_files("library/physics/*.cc")
    add_headerfiles("include/physics/*.h")
    add_includedirs("include", { public = true })
    add_packages("chipmunk2d", { public = true })
    add_deps("gf2core0")

target("gf2imgui0")
    set_kind("static")
    add_defines("GF_IMGUI_STATIC", { public = true })
    add_files("library/imgui/*.cc")
    add_headerfiles("include/imgui/*.h")
    add_includedirs("include", { public = true })
    add_packages("imgui", { public = true })
    add_deps("gf2graphics0")

includes("tests/xmake.lua")
includes("bin/xmake.lua")
includes("examples/xmake.lua")

includes("games/HOME/xmake.lua")
