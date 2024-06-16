option("doc-examples", { description = "Build documentation examples", default = false })

if has_config("doc-examples") then
    set_group("Documentation Examples")

    target("gf2coredoc")
        set_kind("binary")
        add_files("core_*.cc")
        add_files("main.cc")
        add_deps("gf2core0")

    target("gf2graphicsdoc")
        set_kind("binary")
        add_files("graphics_*.cc")
        add_files("main.cc")
        add_deps("gf2graphics0")

    target("gf2networkdoc")
        set_kind("binary")
        add_files("network_*.cc")
        add_files("main.cc")
        add_deps("gf2network0")

    target("gf2manualdoc")
        set_kind("binary")
        add_files("manual_*.cc")
        add_files("main.cc")
        add_deps("gf2framework0", "gf2audio0", "gf2physics0", "gf2imgui0", "gf2graphics0", "gf2core0")

end
