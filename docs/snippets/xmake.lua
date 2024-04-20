option("doc-examples", { description = "Build documentation examples", default = false })

if has_config("doc-examples") then
    set_group("Documentation Examples")

    target("gf2coredoc")
        set_kind("shared")
        add_files("core_*.cc")
        add_deps("gf2core0")

    target("gf2graphicsdoc")
        set_kind("shared")
        add_files("graphics_*.cc")
        add_deps("gf2graphics0")

    target("gf2networkdoc")
        set_kind("shared")
        add_files("network_*.cc")
        add_deps("gf2network0")

end
