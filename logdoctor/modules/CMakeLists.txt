
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/shared.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tb.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tb.cpp
)

add_subdirectory(blacklists)
add_subdirectory(changelog)
add_subdirectory(crapconf)
add_subdirectory(craphelp)
add_subdirectory(crapinfo)
add_subdirectory(craplog)
add_subdirectory(crapup)
add_subdirectory(crapview)
add_subdirectory(database)
add_subdirectory(dialogs)
add_subdirectory(security)
add_subdirectory(warnlists)
