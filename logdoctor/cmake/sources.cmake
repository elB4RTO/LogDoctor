
function(logdoc_add_target_sources)

    target_sources(LogDoctor PRIVATE
        main.cpp
        mainwindow.ui
        mainwindow.h
        mainwindow.cpp
        main_lib.h
    )

    add_subdirectory(customs)
    add_subdirectory(games)
    add_subdirectory(globals)
    add_subdirectory(modules)
    add_subdirectory(tools)
    add_subdirectory(utilities)

    add_subdirectory(resources)

endfunction()
