option("binaries", { description = "Build binaries", default = true })

if has_config("binaries") then
    set_group("Binaries")

    target("gf2_glyph_display")
        set_kind("binary")
        add_files("gf2_glyph_display.cc")
        add_deps("gf2core0")
        set_rundir("$(projectdir)")

    target("gf2_namegen")
        set_kind("binary")
        add_files("gf2_namegen.cc")
        add_deps("gf2core0")
        set_rundir("$(projectdir)")

    target("gf2_play_sound")
        set_kind("binary")
        add_files("gf2_play_sound.cc")
        add_deps("gf2audio0")
        set_rundir("$(projectdir)")

    target("gf2_tarball_listfiles")
        set_kind("binary")
        add_files("gf2_tarball_listfiles.cc")
        add_deps("gf2core0")
        set_rundir("$(projectdir)")

    target("gf2_tmx_display")
        set_kind("binary")
        add_files("gf2_tmx_display.cc")
        add_deps("gf2graphics0")
        set_rundir("$(projectdir)")

end
