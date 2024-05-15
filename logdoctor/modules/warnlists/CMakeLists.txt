
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/warnlists.h
    ${CMAKE_CURRENT_SOURCE_DIR}/warnlists.cpp
)

add_subdirectory(modules)
