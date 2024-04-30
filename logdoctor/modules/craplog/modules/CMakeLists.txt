
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/donuts.h
    ${CMAKE_CURRENT_SOURCE_DIR}/donuts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formats.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hasher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hasher.cpp
)

add_subdirectory(formats)
add_subdirectory(hasher)
