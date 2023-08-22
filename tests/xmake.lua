option("tests", { description = "Build tests", default = true })

if has_config("tests") then
    set_group("Tests")

    target("gf2_core_tests")
        set_kind("binary")
        set_languages("cxx17")
        add_files("tests_core_*.cc", "main_core.cc")
        add_files("googletest/googletest/src/gtest-all.cc", { warnings = "none" })
        add_includedirs("googletest/googletest")
        add_includedirs("googletest/googletest/include")
        add_deps("gf2core0")
        if is_plat("linux") then
            add_syslinks("pthread")
        end
        set_configdir("$(buildir)/config/core_tests")
        set_configvar("GF_TEST_ASSETS_DIRECTORY", "$(projectdir)/tests/assets")
        add_configfiles("config.h.in", {pattern = "@(.-)@"})
        add_includedirs("$(buildir)/config/core_tests")

    target("gf2_network_tests")
        set_kind("binary")
        set_languages("cxx17")
        add_files("tests_network_*.cc", "main_network.cc")
        add_files("googletest/googletest/src/gtest-all.cc", { warnings = "none" })
        add_includedirs("googletest/googletest")
        add_includedirs("googletest/googletest/include")
        add_deps("gf2network0")
        if is_plat("linux") then
            add_syslinks("pthread")
        end

    target("gf2_audio_tests")
        set_kind("binary")
        set_languages("cxx17")
        add_files("tests_audio_*.cc", "main_audio.cc")
        add_files("googletest/googletest/src/gtest-all.cc", { warnings = "none" })
        add_includedirs("googletest/googletest")
        add_includedirs("googletest/googletest/include")
        add_deps("gf2audio0")
        if is_plat("linux") then
            add_syslinks("pthread")
        end
        set_configdir("$(buildir)/config/audio_tests")
        set_configvar("GF_TEST_ASSETS_DIRECTORY", "$(projectdir)/tests/assets")
        add_configfiles("config.h.in", {pattern = "@(.-)@"})
        add_includedirs("$(buildir)/config/audio_tests")

end
