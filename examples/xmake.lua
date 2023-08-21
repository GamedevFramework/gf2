option("examples", { description = "Build examples", default = true })

if has_config("examples") then
    set_group("Examples")

    target("00-triangle")
        set_kind("binary")
        add_files("00-triangle.cc")
        add_deps("gf2graphics0")

end
