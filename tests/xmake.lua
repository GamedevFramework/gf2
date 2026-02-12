option("tests", { description = "Build tests", default = true })

if has_config("tests") then
    set_group("Tests")

    target("gf2_core_tests")
        set_kind("binary")
        add_files("tests_core_*.cc", "main_core.cc")
        add_deps("gf2core0")
        add_packages("gtest")
        set_configdir("$(builddir)/config/core_tests")
        set_configvar("GF_TEST_ASSETS_DIRECTORY", "$(projectdir)/tests/assets")
        add_configfiles("config.h.in", {pattern = "@(.-)@"})
        add_includedirs("$(builddir)/config/core_tests")
        add_tests("default")

    if has_config("network") then
        target("gf2_network_tests")
            set_kind("binary")
            add_files("tests_network_*.cc", "main_network.cc")
            add_deps("gf2network0")
            add_packages("gtest")
            add_tests("default")
    end

    if has_config("audio") then
        target("gf2_audio_tests")
            set_kind("binary")
            add_files("tests_audio_*.cc", "main_audio.cc")
            add_deps("gf2audio0")
            add_packages("gtest")
            set_configdir("$(builddir)/config/audio_tests")
            set_configvar("GF_TEST_ASSETS_DIRECTORY", "$(projectdir)/tests/assets")
            add_configfiles("config.h.in", {pattern = "@(.-)@"})
            add_includedirs("$(builddir)/config/audio_tests")
            add_tests("default")
    end

end
