
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/craplog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/craplog.cpp
)

add_subdirectory(modules)
add_subdirectory(utilities)
add_subdirectory(workers)
