
set_languages("cxx17")

target("im-select-mspy")
    set_kind("binary")
    add_files("main.cc")
    add_cxxflags("/utf-8")
    add_links("comsuppw.lib")
