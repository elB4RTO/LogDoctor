cmake_minimum_required(VERSION 3.5)

project(LogDoctor VERSION 6.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts LinguistTools Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts LinguistTools Sql)


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

        utilities.h
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
        utilities/vectors.h
        utilities/vectors.cpp

        modules.h
        modules/charts/donuts.h
        modules/charts/donuts.cpp
        modules/checks.h
        modules/checks.cpp
        modules/dialogs.h
        modules/dialogs.cpp
        modules/exceptions.h
        modules/exceptions.cpp
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

        tools/craphelp/craphelp.ui
        tools/craphelp/craphelp.h
        tools/craphelp/craphelp.cpp

        tools/crapup/crapup.h
        tools/crapup/crapup.cpp

        tools/crapnote/crapnote.ui
        tools/crapnote/crapnote.h
        tools/crapnote/crapnote.cpp

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

target_link_libraries(LogDoctor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts Qt${QT_VERSION_MAJOR}::Sql)

# Include zlib
find_package( ZLIB REQUIRED )
include_directories(${ZLIB_INCLUDE_DIRS})
target_link_libraries(LogDoctor PRIVATE ${ZLIB_LIBRARIES})

# Include libcurl
find_package( CURL REQUIRED )
include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(LogDoctor PRIVATE ${CURL_LIBRARIES})

set_target_properties(LogDoctor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LogDoctor)
endif()
