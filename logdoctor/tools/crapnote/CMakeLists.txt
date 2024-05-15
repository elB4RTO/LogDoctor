
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/crapnote.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/crapnote.h
    ${CMAKE_CURRENT_SOURCE_DIR}/crapnote.cpp
)

add_subdirectory(modules)
