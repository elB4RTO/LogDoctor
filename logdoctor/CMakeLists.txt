cmake_minimum_required(VERSION 3.5)

project(LogDoctor VERSION 2.01 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts LinguistTools Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts LinguistTools Sql Network)


set(TS_FILES
    translations/LogDoctor_en.ts
    translations/LogDoctor_es.ts
    translations/LogDoctor_fr.ts
    translations/LogDoctor_it.ts
)


set(PROJECT_SOURCES
    main.cpp
    mainwindow.ui
    mainwindow.h
    mainwindow.cpp

    utilities/checks.h
    utilities/checks.cpp
    utilities/colors.h
    utilities/colors.cpp
    utilities/gzip.h
    utilities/gzip.cpp
    utilities/io.h
    utilities/io.cpp
    utilities/rtf.h
    utilities/rtf.cpp
    utilities/strings.h
    utilities/strings.cpp
    utilities/stylesheets.h
    utilities/stylesheets.cpp
    utilities/vectors.h
    utilities/vectors.cpp

    modules/shared.h
    modules/shared.cpp

    modules/exceptions.h
    modules/exceptions.cpp
    modules/tb.h
    modules/tb.cpp

    modules/dialogs.h
    modules/dialogs.cpp
    modules/dialogs/dialogmsg.ui
    modules/dialogs/dialogmsg.h
    modules/dialogs/dialogmsg.cpp
    modules/dialogs/dialogbool.ui
    modules/dialogs/dialogbool.h
    modules/dialogs/dialogbool.cpp
    modules/dialogs/dialogdia.ui
    modules/dialogs/dialogdia.h
    modules/dialogs/dialogdia.cpp

    modules/craplog/craplog.h
    modules/craplog/craplog.cpp
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
    modules/craplog/modules/store.h
    modules/craplog/modules/store.cpp

    modules/crapview/crapview.h
    modules/crapview/crapview.cpp
    modules/crapview/modules/query.h
    modules/crapview/modules/query.cpp

    modules/craphelp/craphelp.ui
    modules/craphelp/craphelp.h
    modules/craphelp/craphelp.cpp

    modules/crapup/crapup.ui
    modules/crapup/crapup.h
    modules/crapup/crapup.cpp

    modules/crapinfo/crapinfo.ui
    modules/crapinfo/crapinfo.h
    modules/crapinfo/crapinfo.cpp

    tools/crapnote/crapnote.ui
    tools/crapnote/crapnote.h
    tools/crapnote/crapnote.cpp

    games/games.h
    games/games.cpp

    games/crisscross/crisscross.ui
    games/crisscross/game.h
    games/crisscross/game.cpp

    games/snake/snake.ui
    games/snake/game.h
    games/snake/game.cpp
    games/snake/snake.h
    games/snake/snake.cpp
    games/snake/food.h
    games/snake/food.cpp

    resources/resources.qrc
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LogDoctor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET LogDoctor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(LogDoctor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LogDoctor
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic -lgcc -lstdc++ -lwinpthread")
    target_link_libraries(LogDoctor PRIVATE
        -static Qt${QT_VERSION_MAJOR}::Widgets
        -static Qt${QT_VERSION_MAJOR}::Charts
        -static Qt${QT_VERSION_MAJOR}::Sql
        -static Qt${QT_VERSION_MAJOR}::Network)
else()
    target_link_libraries(LogDoctor PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Charts
        Qt${QT_VERSION_MAJOR}::Sql
        Qt${QT_VERSION_MAJOR}::Network)
    # Include zlib
    find_package(ZLIB REQUIRED)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries(LogDoctor PRIVATE ${ZLIB_LIBRARIES})
endif()


if(APPLE)
    set_target_properties(LogDoctor PROPERTIES
        MACOSX_BUNDLE TRUE
        #MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_ICON_FILE ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
elseif(WIN32)
    set_target_properties(LogDoctor PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LogDoctor)
endif()


# Flatpak
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
        ${CMAKE_SOURCE_DIR}/installation_stuff/logdoctata/help
            DESTINATION /usr/share/LogDoctor
    )
    install(DIRECTORY
        ${CMAKE_SOURCE_DIR}/installation_stuff/logdoctata/licenses
            DESTINATION /usr/share/LogDoctor
    )
endif()
