cmake_minimum_required(VERSION 3.5)

project(LogDoctor VERSION 3.00 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Charts LinguistTools Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts LinguistTools Sql Network)

if(${QT_VERSION_MAJOR} LESS 6)
    message("Error: Qt6 or greater required")
    return()
endif()


set(TS_FILES
    translations/LogDoctor_en_GB.ts
    translations/LogDoctor_es_ES.ts
    translations/LogDoctor_fr_FR.ts
    translations/LogDoctor_it_IT.ts
    translations/LogDoctor_ja_JP.ts
    translations/LogDoctor_pt_BR.ts
)


set(PROJECT_SOURCES
    main.cpp
    mainwindow.ui
    mainwindow.h
    mainwindow.cpp

    globals/global_configs.h
    globals/global_configs.cpp

    defines/web_servers.h

    customs/treewidgetitems.h

    utilities/checks.h
    utilities/checks.cpp
    utilities/colors.h
    utilities/colors.cpp
    utilities/gzip.h
    utilities/gzip.cpp
    utilities/io.h
    utilities/io.cpp
    utilities/memory.h
    utilities/memory.cpp
    utilities/printables.h
    utilities/printables.cpp
    utilities/rtf.h
    utilities/rtf.cpp
    utilities/strings.h
    utilities/strings.cpp
    utilities/stylesheets.h
    utilities/stylesheets.cpp
    utilities/vectors.h

    modules/shared.h
    modules/shared.cpp

    modules/exceptions.h
    modules/exceptions.cpp
    modules/tb.h
    modules/tb.cpp

    modules/dialogs.h
    modules/dialogs.cpp
    modules/dialogs/message_dialog.ui
    modules/dialogs/message_dialog.h
    modules/dialogs/message_dialog.cpp
    modules/dialogs/boolean_dialog.ui
    modules/dialogs/boolean_dialog.h
    modules/dialogs/boolean_dialog.cpp
    modules/dialogs/ida_dialog.ui
    modules/dialogs/ida_dialog.h
    modules/dialogs/ida_dialog.cpp

    modules/craplog/craplog.h
    modules/craplog/craplog.cpp
    modules/craplog/modules/lib.h
    modules/craplog/modules/datetime.h
    modules/craplog/modules/datetime.cpp
    modules/craplog/modules/donuts.h
    modules/craplog/modules/donuts.cpp
    modules/craplog/modules/formats.h
    modules/craplog/modules/formats.cpp
    modules/craplog/modules/hash.h
    modules/craplog/modules/hash.cpp
    modules/craplog/modules/logs.h
    modules/craplog/modules/logs.cpp
    modules/craplog/modules/sha256.h
    modules/craplog/modules/sha256.cpp
    modules/craplog/modules/workers/lib.h
    modules/craplog/modules/workers/lister.h
    modules/craplog/modules/workers/lister.cpp
    modules/craplog/modules/workers/parser_interface.h
    modules/craplog/modules/workers/parser_interface.cpp
    modules/craplog/modules/workers/parser.h
    modules/craplog/modules/workers/parser.cpp
    modules/craplog/modules/workers/parser_async.h
    modules/craplog/modules/workers/parser_async.cpp

    modules/crapview/crapview.h
    modules/crapview/crapview.cpp
    modules/crapview/modules/filters.h
    modules/crapview/modules/filters.cpp
    modules/crapview/modules/query.h
    modules/crapview/modules/query.cpp

    modules/craphelp/craphelp.ui
    modules/craphelp/craphelp.h
    modules/craphelp/craphelp.cpp

    modules/crapup/crapup.ui
    modules/crapup/crapup.h
    modules/crapup/crapup.cpp
    modules/crapup/modules/stylesheets.h
    modules/crapup/modules/stylesheets.cpp

    modules/crapinfo/crapinfo.ui
    modules/crapinfo/crapinfo.h
    modules/crapinfo/crapinfo.cpp
    modules/crapinfo/modules/stylesheets.h
    modules/crapinfo/modules/stylesheets.cpp

    tools/crapnote/crapnote.ui
    tools/crapnote/crapnote.h
    tools/crapnote/crapnote.cpp

    games/game_dialog.ui
    games/game_dialog.h
    games/game_dialog.cpp
    games/crisscross/crisscross.ui
    games/crisscross/game.h
    games/crisscross/game.cpp
    games/crisscross/modules/stylesheets.h
    games/crisscross/modules/stylesheets.cpp

    games/snake/snake.ui
    games/snake/game.h
    games/snake/game.cpp
    games/snake/snake.h
    games/snake/snake.cpp
    games/snake/food.h
    games/snake/food.cpp
    games/snake/modules/stylesheets.h
    games/snake/modules/stylesheets.cpp

    resources/resources.qrc
    ${TS_FILES}
)

qt_add_executable(LogDoctor
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic -lgcc -lstdc++ -lwinpthread")
    target_link_libraries(LogDoctor PRIVATE
        -static Qt${QT_VERSION_MAJOR}::Widgets
        -static Qt${QT_VERSION_MAJOR}::Charts
        -static Qt${QT_VERSION_MAJOR}::Sql
        -static Qt${QT_VERSION_MAJOR}::Network
    )
else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
    target_link_libraries(LogDoctor PRIVATE
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

set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

if(MSVC)
    add_compile_options(/W2)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
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

qt_finalize_executable(LogDoctor)



# CTest
include(CTest)
enable_testing()
add_executable(LogDocTester
    tests/main.cpp
    tests/white_box.h
    tests/white_box.cpp
    # 2 test
    modules/exceptions.h
    modules/exceptions.cpp
    modules/craplog/modules/lib.h
    utilities/gzip.h
    utilities/gzip.cpp
    utilities/io.h
    utilities/io.cpp
    utilities/strings.h
    utilities/strings.cpp
    utilities/vectors.h
    modules/craplog/modules/datetime.h
    modules/craplog/modules/datetime.cpp
    modules/craplog/modules/formats.h
    modules/craplog/modules/formats.cpp
    modules/craplog/modules/logs.h
    modules/craplog/modules/logs.cpp
    modules/crapview/modules/filters.h
    modules/crapview/modules/filters.cpp
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
