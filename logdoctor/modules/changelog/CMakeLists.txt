
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/changelog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/changelog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/changelog.cpp
)

add_subdirectory(modules)
