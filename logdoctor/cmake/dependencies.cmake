
macro(logdoc_find_required_packages)

    find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets Charts LinguistTools Sql Network)
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts LinguistTools Sql Network)

    # zlib
    if(NOT WIN32)
        find_package(ZLIB REQUIRED)
        include_directories(${ZLIB_INCLUDE_DIRS})
    endif()

endmacro()
