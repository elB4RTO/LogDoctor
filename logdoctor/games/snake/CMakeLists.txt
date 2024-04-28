
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/snake.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/game.h
    ${CMAKE_CURRENT_SOURCE_DIR}/game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/snake.h
    ${CMAKE_CURRENT_SOURCE_DIR}/snake.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/food.h
    ${CMAKE_CURRENT_SOURCE_DIR}/food.cpp
)

add_subdirectory(modules)
