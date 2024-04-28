
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/crapup.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/crapup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/crapup.cpp
)

add_subdirectory(modules)
