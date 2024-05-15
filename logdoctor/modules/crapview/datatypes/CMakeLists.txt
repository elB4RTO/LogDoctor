
target_sources(LogDoctor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/fwd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/count_stats.h
    ${CMAKE_CURRENT_SOURCE_DIR}/count_stats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/daytime_stats.h
    ${CMAKE_CURRENT_SOURCE_DIR}/daytime_stats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/global_stats.h
    ${CMAKE_CURRENT_SOURCE_DIR}/relational_stats.h
    ${CMAKE_CURRENT_SOURCE_DIR}/relational_stats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/speed_stats.h
    ${CMAKE_CURRENT_SOURCE_DIR}/speed_stats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/warning_stats.h
    ${CMAKE_CURRENT_SOURCE_DIR}/warning_stats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/typedefs.h
)
