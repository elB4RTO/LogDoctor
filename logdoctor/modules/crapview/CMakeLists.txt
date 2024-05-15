
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/crapview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/crapview.cpp
)

add_subdirectory(datatypes)
add_subdirectory(modules)
add_subdirectory(utilities)
