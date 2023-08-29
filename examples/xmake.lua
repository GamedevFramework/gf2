option("examples", { description = "Build examples", default = true })

if has_config("examples") then
    set_group("Examples")

    set_configdir("$(buildir)/config/examples")
    set_configvar("GF_EXAMPLE_ASSETS_DIRECTORY", "$(projectdir)/examples/assets")
    add_configfiles("config.h.in", {pattern = "@(.-)@"})

    target("00-triangle")
        set_kind("binary")
        add_files("00-triangle.cc")
        add_deps("gf2graphics0")

    target("01-rectangle")
        set_kind("binary")
        add_files("01-rectangle.cc")
        add_deps("gf2graphics0")

    target("02-texture")
        set_kind("binary")
        add_files("02-texture.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(buildir)/config/examples")

    target("03-camera")
        set_kind("binary")
        add_files("03-camera.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(buildir)/config/examples")
end
