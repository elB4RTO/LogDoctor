cmake_minimum_required(VERSION 3.5)

project(Craplog VERSION 6.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts LinguistTools Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts LinguistTools Sql)


set(TS_FILES
        translations/Craplog_es_ES.ts
        translations/Craplog_fr_FR.ts
        translations/Craplog_it_IT.ts
)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.h
        mainwindow.cpp
        mainwindow.ui

        utilities.h
        utilities/colors.h
        utilities/colors.cpp
        utilities/io.h
        utilities/io.cpp
        utilities/rtf.h
        utilities/rtf.cpp
        utilities/strings.h
        utilities/strings.cpp
        utilities/vectors.h
        utilities/vectors.cpp

        modules.h
        modules/charts/donuts.h
        modules/charts/donuts.cpp
        modules/checks.h
        modules/checks.cpp
        modules/dialogs.h
        modules/dialogs.cpp
        modules/tb.h
        modules/tb.cpp

        tools/craplog/craplog.h
        tools/craplog/craplog.cpp
        tools/craplog/modules/datetime.h
        tools/craplog/modules/datetime.cpp
        tools/craplog/modules/formats.h
        tools/craplog/modules/formats.cpp
        tools/craplog/modules/hash.h
        tools/craplog/modules/hash.cpp
        tools/craplog/modules/logs.h
        tools/craplog/modules/logs.cpp
        tools/craplog/modules/sha256.h
        tools/craplog/modules/sha256.cpp
        tools/craplog/modules/store.h
        tools/craplog/modules/store.cpp

        tools/crapview/crapview.h
        tools/crapview/crapview.cpp
        tools/crapview/modules/query.h
        tools/crapview/modules/query.cpp

        resources/resources.qrc
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Craplog
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Craplog APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Craplog SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Craplog
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Craplog PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts Qt${QT_VERSION_MAJOR}::Sql)

set_target_properties(Craplog PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Craplog)
endif()
