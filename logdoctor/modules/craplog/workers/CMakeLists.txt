
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lister.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lister.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp
)

add_subdirectory(parser)
