cmake_minimum_required(VERSION 3.5)

project(LogDoctor VERSION 4.00 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets Charts LinguistTools Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Charts LinguistTools Sql Network)

if(${QT_VERSION_MAJOR} LESS 6)
    message("Error: Qt6 or greater required")
    return()
endif()


include(cmake/main.cmake)
include(cmake/customs.cmake)
include(cmake/games.cmake)
include(cmake/globals.cmake)
include(cmake/modules.cmake)
include(cmake/tools.cmake)
include(cmake/utilities.cmake)


set(PROJECT_SOURCES
    ${MAIN_SOURCES}
    ${GLOBALS_SOURCES}
    ${CUSTOMS_SOURCES}
    ${UTILITIES_SOURCES}
    ${MODULES_SOURCES}
    ${TOOLS_SOURCES}
    ${GAMES_SOURCES}
    resources/resources.qrc
)

qt_add_executable(LogDoctor
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

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
    )
    # Include zlib
    find_package(ZLIB REQUIRED)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries(LogDoctor PRIVATE ${ZLIB_LIBRARIES})
endif()

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


include(cmake/translations.cmake)
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})


qt_finalize_executable(LogDoctor)



# CTest
include(CTest)
enable_testing()
include(cmake/tests.cmake)
add_executable(LogDocTester
    tests/main.cpp
    # suites
    tests/lib.h
    tests/white_box.h
    tests/white_box.cpp
    # to test
    ${TO_TESTS_SOURCES}
)
if(WIN32)
    target_link_libraries(LogDocTester PRIVATE
        -static Qt${QT_VERSION_MAJOR}::Widgets
        #-static Qt${QT_VERSION_MAJOR}::Sql
    )
else()
    target_link_libraries(LogDocTester PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        #Qt${QT_VERSION_MAJOR}::Sql
        ${ZLIB_LIBRARIES}
    )
endif()
add_test(
  NAME LogDocTester
  COMMAND $<TARGET_FILE:LogDocTester>
)


# Deb/Flatpak
if(UNIX AND NOT APPLE)
    install(TARGETS
        LogDoctor
            RUNTIME DESTINATION /usr/bin
            CONFIGURATIONS Release
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ
            RENAME logdoctor
    )

    install(FILES
        ${CMAKE_SOURCE_DIR}/installation_stuff/LogDoctor.desktop
            DESTINATION /usr/share/applications
    )

    install(DIRECTORY
        DESTINATION /usr/share/LogDoctor
    )
    install(FILES
        ${CMAKE_SOURCE_DIR}/installation_stuff/LogDoctor.svg
            DESTINATION /usr/share/LogDoctor
    )
    install(DIRECTORY
        ${CMAKE_SOURCE_DIR}/installation_stuff/logdocdata/help
            DESTINATION /usr/share/LogDoctor
    )
    install(DIRECTORY
        ${CMAKE_SOURCE_DIR}/installation_stuff/logdocdata/licenses
            DESTINATION /usr/share/LogDoctor
    )
endif()
