
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/game_dialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/game_dialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/game_dialog.cpp
)

add_subdirectory(crisscross)
add_subdirectory(snake)
