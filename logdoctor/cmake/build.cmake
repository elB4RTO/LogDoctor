
macro(logdoc_set_compiler_options)

    if(MSVC)
        add_compile_options(/W2)
        set(CMAKE_CXX_FLAGS_MINSIZEREL "/Os")
        set(CMAKE_CXX_FLAGS_RELEASE "/O2")
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    endif()

    if(CMAKE_BUILD_TYPE EQUAL "RELEASE")
        include(CheckIPOSupported)
        check_ipo_supported(RESULT supported)
        if(supported)
            set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        endif()
    endif()

endmacro()


macro(logdoc_set_linker_options)

    if(WIN32)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic -lstdc++ -lwinpthread")
        target_link_libraries(LogDoctor PRIVATE
            -static Qt${QT_VERSION_MAJOR}::Core
            -static Qt${QT_VERSION_MAJOR}::Widgets
            -static Qt${QT_VERSION_MAJOR}::Charts
            -static Qt${QT_VERSION_MAJOR}::Sql
            -static Qt${QT_VERSION_MAJOR}::Network
        )
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
        target_link_libraries(LogDoctor PRIVATE
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::Charts
            Qt${QT_VERSION_MAJOR}::Sql
            Qt${QT_VERSION_MAJOR}::Network
            ${ZLIB_LIBRARIES}
        )
    endif()

endmacro()


macro(logdoc_set_target_properties)

    if(WIN32)
        set_target_properties(LogDoctor PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
    elseif(APPLE)
        set_target_properties(LogDoctor PROPERTIES
            MACOSX_BUNDLE TRUE
            #MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
            MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
            MACOSX_BUNDLE_ICON_FILE ${PROJECT_NAME}
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        )
    endif()

endmacro()
