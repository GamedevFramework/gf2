option("games", { description = "Build games", default = true })

if has_config("games") then
    set_group("Games")

    set_configdir("$(buildir)/config/games/HOME")
    set_configvar("GF_HOME_ASSETS_DIRECTORY", "$(projectdir)/games/HOME/assets")
    add_configfiles("config.h.in", {pattern = "@(.-)@"})

    target("HOME")
        set_kind("binary")
        add_files("HOME.cc")
        add_files("bits/*.cc")
        add_deps("gf2audio0")
        add_deps("gf2physics0")
        add_deps("gf2graphics0")
        add_deps("gf2framework0")
        add_includedirs("$(buildir)/config/games/HOME")

end
