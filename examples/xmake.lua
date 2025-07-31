option("examples", { description = "Build examples", default = true })

if has_config("examples") then
    set_group("Examples")

    set_configdir("$(builddir)/config/examples")
    set_configvar("GF_EXAMPLE_ASSETS_DIRECTORY", "$(projectdir)/examples/assets")
    add_configfiles("config.h.in", {pattern = "@(.-)@"})

    target("01-colors")
        set_kind("binary")
        add_files("01-colors.cc")
        add_deps("gf2core0")
        set_rundir("$(projectdir)/docs/assets/colors")

    target("events")
        set_kind("binary")
        add_files("events.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(builddir)/config/examples")

    target("hud")
        set_kind("binary")
        add_files("hud.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(builddir)/config/examples")

    target("10-sprite")
        set_kind("binary")
        add_files("10-sprite.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(builddir)/config/examples")

    target("ninepatch")
        set_kind("binary")
        add_files("ninepatch.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(builddir)/config/examples")

    target("11-text")
        set_kind("binary")
        add_files("11-text.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(builddir)/config/examples")

    target("12-animation")
        set_kind("binary")
        add_files("12-animation.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(builddir)/config/examples")

    target("13-shape")
        set_kind("binary")
        add_files("13-shape.cc")
        add_deps("gf2graphics0")

    target("14-curve")
        set_kind("binary")
        add_files("14-curve.cc")
        add_deps("gf2graphics0")

    target("15-imgui")
        set_kind("binary")
        add_files("15-imgui.cc")
        add_deps("gf2imgui0")
        add_includedirs("$(builddir)/config/examples")

    target("16-richtext")
        set_kind("binary")
        add_files("16-richtext.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(builddir)/config/examples")

    target("17-console")
        set_kind("binary")
        add_files("17-console.cc")
        add_deps("gf2graphics0")
        add_includedirs("$(builddir)/config/examples")

    target("18-activities")
        set_kind("binary")
        add_files("18-activities.cc")
        add_deps("gf2graphics0")

    target("19-gridmap")
        set_kind("binary")
        add_files("19-gridmap.cc")
        add_deps("gf2graphics0")

    target("20-physics")
        set_kind("binary")
        add_files("20-physics.cc")
        add_deps("gf2physics0")
        add_deps("gf2graphics0")
end
