set_project("gf2")
set_version("0.1.0")

add_requires("libsdl", "freetype", "harfbuzz", "imgui >= 1.91.4", "pugixml", "zlib")
add_requires("chipmunk2d")
add_requires("fmt")
add_requires("gtest", { configs = { gmock = false }})
add_requires("miniaudio")
add_requires("stb", { system = false })
add_requires("vk-bootstrap")
add_requires("volk")
add_requires("vulkan-headers")
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
    set_kind("$(kind)")
    if is_kind("static") then
        add_defines("GF_CORE_STATIC", { public = true })
    else
        add_defines("GF_CORE_BUILD")
    end
    add_defines("ZLIB_CONST")
    add_files("library/core/*.cc")
    add_headerfiles("include/(gf2/core/*.h)")
    add_includedirs("include", { public = true })
    add_packages("freetype", "pugixml", "stb")
    add_packages("fmt", "zlib", { public = true })
    set_license("Zlib")

target("gf2graphics0")
    set_kind("$(kind)")
    if is_kind("static") then
        add_defines("GF_GRAPHICS_STATIC", { public = true })
    else
        add_defines("GF_GRAPHICS_BUILD")
    end
    add_defines("VK_NO_PROTOTYPES", { public = true })
    add_files("library/graphics/*.cc")
    add_headerfiles("include/(gf2/graphics/*.h)")
    add_includedirs("include", { public = true })
    add_packages("vulkan-headers", "vulkan-memory-allocator", { public = true })
    add_packages("harfbuzz", "libsdl", "volk", "vk-bootstrap")
    add_includedirs("library/graphics/shaders")
    add_deps("gf2core0")
    set_license("Zlib")

target("gf2network0")
    set_kind("$(kind)")
    if is_kind("static") then
        add_defines("GF_NETWORK_STATIC", { public = true })
    else
        add_defines("GF_NETWORK_BUILD")
    end
    add_files("library/network/*.cc")
    add_files("library/network/bits/*.cc")
    add_headerfiles("include/(gf2/network/*.h)")
    add_includedirs("include", { public = true })
    add_deps("gf2core0")
    if is_plat("windows") then
        add_syslinks("ws2_32")
    end
    set_license("Zlib")

target("gf2audio0")
    set_kind("$(kind)")
    if is_kind("static") then
        add_defines("GF_AUDIO_STATIC", { public = true })
    else
        add_defines("GF_AUDIO_BUILD")
    end
    add_files("library/audio/*.cc")
    add_headerfiles("include/(gf2/audio/*.h)")
    add_files("library/audio/bits/*.cc", { warnings = "none" })
    add_includedirs("include", { public = true })
    add_packages("miniaudio", "stb")
    add_deps("gf2core0")
    if is_plat("linux") then
        add_syslinks("dl")
    end
    set_license("Zlib")

target("gf2physics0")
    set_kind("$(kind)")
    if is_kind("static") then
        add_defines("GF_PHYSICS_STATIC", { public = true })
    else
        add_defines("GF_PHYSICS_BUILD")
    end
    add_files("library/physics/*.cc")
    add_headerfiles("include/(gf2/physics/*.h)")
    add_includedirs("include", { public = true })
    add_packages("chipmunk2d", { public = true })
    add_deps("gf2core0")
    set_license("Zlib")

target("gf2imgui0")
    set_kind("static") -- keep it static because of imgui
    add_defines("GF_IMGUI_STATIC", { public = true })
    add_files("library/imgui/*.cc")
    add_headerfiles("include/(gf2/imgui/*.h)")
    add_includedirs("include", { public = true })
    add_packages("imgui", { public = true })
    add_deps("gf2graphics0")
    set_license("Zlib")

target("gf2framework0")
    set_kind("$(kind)")
    if is_kind("static") then
        add_defines("GF_FRAMEWORK_STATIC", { public = true })
    else
        add_defines("GF_FRAMEWORK_BUILD")
    end
    add_files("library/framework/*.cc")
    add_headerfiles("include/(gf2/framework/*.h)")
    add_includedirs("include", { public = true })
    add_deps("gf2core0")
    add_deps("gf2graphics0")
    add_deps("gf2audio0")
    add_deps("gf2physics0")
    set_license("Zlib")

includes("tests/xmake.lua")
includes("bin/xmake.lua")
includes("examples/xmake.lua")

includes("games/HOME/xmake.lua")

includes("docs/snippets/xmake.lua")
