
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/blacklists.h
    ${CMAKE_CURRENT_SOURCE_DIR}/blacklists.cpp
)

add_subdirectory(modules)
