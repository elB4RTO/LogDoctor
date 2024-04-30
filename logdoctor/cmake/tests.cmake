
macro(logdoc_add_tests)

    include(CTest)

    enable_testing()

    add_executable(LogDocTester
        tests/main.cpp
        # suites
        tests/lib.h
        tests/white_box.h
        tests/white_box.cpp
        # to test
        modules/exceptions.h
        modules/exceptions.cpp

        modules/craplog/modules/lib.h
        modules/craplog/modules/formats/parse.h
        modules/craplog/modules/formats/parse.cpp
        modules/craplog/utilities/datetime.h
        modules/craplog/utilities/datetime.cpp
        modules/craplog/utilities/logs.h
        modules/craplog/utilities/logs.cpp
        modules/craplog/workers/lib.h
        modules/craplog/workers/parser/logs_data.h
        modules/craplog/workers/parser/logs_data.cpp

        modules/crapview/utilities/filters.h
        modules/crapview/utilities/filters.cpp

        utilities/gzip.h
        utilities/gzip.cpp
        utilities/io.h
        utilities/io.cpp
        utilities/strings.h
        utilities/strings.cpp
        utilities/vectors.h
    )

    if(WIN32)
        target_link_libraries(LogDocTester PRIVATE
            -static Qt${QT_VERSION_MAJOR}::Widgets
            #-static Qt${QT_VERSION_MAJOR}::Sql
        )
    else()
        target_link_libraries(LogDocTester PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            #Qt${QT_VERSION_MAJOR}::Sql
            ${ZLIB_LIBRARIES}
        )
    endif()

    add_test(
      NAME LogDocTester
      COMMAND $<TARGET_FILE:LogDocTester>
    )

endmacro()
