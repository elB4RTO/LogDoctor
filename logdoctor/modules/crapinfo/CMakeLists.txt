
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/crapinfo.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/crapinfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/crapinfo.cpp
)

add_subdirectory(modules)
