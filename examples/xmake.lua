option("examples", { description = "Build examples", default = true })

if has_config("examples") then
    set_group("Examples")

    set_configdir("$(buildir)/config/examples")
    set_configvar("GF_EXAMPLE_ASSETS_DIRECTORY", "$(projectdir)/examples/assets")
    add_configfiles("config.h.in", {pattern = "@(.-)@"})

    target("10-sprite")
        set_kind("binary")
        add_files("10-sprite.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(buildir)/config/examples")

    target("11-text")
        set_kind("binary")
        add_files("11-text.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(buildir)/config/examples")

    target("12-animation")
        set_kind("binary")
        add_files("12-animation.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(buildir)/config/examples")

    target("13-shape")
        set_kind("binary")
        add_files("13-shape.cc")
        add_deps("gf2graphics0")


    target("15-imgui")
        set_kind("binary")
        add_files("15-imgui.cc")
        add_deps("gf2imgui0")
        add_includedirs("$(buildir)/config/examples")
end
