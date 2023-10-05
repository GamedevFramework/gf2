set_project("gf2")
set_version("0.1.0")

add_requires("libsdl", "freetype", "pugixml", "zlib")
add_requires("fmt", { system = false, configs = { header_only = true }})
add_requires("glslang", { configs = { binaryonly = true }})
add_requires("miniaudio 0.11.17")
add_requires("stb", { system = false })
add_requires("vk-bootstrap")
add_requires("volk", { configs = { header_only = true }})
add_requires("vulkan-headers")
-- add_requires("vulkan-validationlayers")
add_requires("vulkan-memory-allocator")

add_rules("mode.check", "mode.coverage", "mode.debug", "mode.releasedbg", "mode.release")

set_policy("build.warning", true)
set_warnings("allextra")
set_languages("cxx17")
set_symbols("hidden")
set_encodings("utf-8")

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
    add_packages("vulkan-headers", "volk", "vulkan-memory-allocator", { public = true })
    add_packages("libsdl", "vk-bootstrap") -- , "vulkan-validationlayers", "glslang"
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


includes("tests/xmake.lua")
includes("bin/xmake.lua")
includes("examples/xmake.lua")
