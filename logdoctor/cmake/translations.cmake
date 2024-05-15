
set(TS_FILES
    translations/LogDoctor_en_GB.ts
    translations/LogDoctor_es_ES.ts
    translations/LogDoctor_fr_FR.ts
    translations/LogDoctor_it_IT.ts
    translations/LogDoctor_ja_JP.ts
    translations/LogDoctor_pt_BR.ts
)

macro(logdoc_add_lupdate_target)

    if(${QT_VERSION_MINOR} LESS 7)
        qt_add_lupdate(LogDoctor
            TS_FILES ${TS_FILES}
            INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}
            OPTIONS "-no-obsolete"
        )
    else()
        qt_add_lupdate(
            SOURCE_TARGETS LogDoctor
            TS_FILES ${TS_FILES}
            INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}
            OPTIONS "-no-obsolete"
        )
    endif()

endmacro()


macro(logdoc_add_lrelease_target)

    if(${QT_VERSION_MINOR} LESS 7)
        qt_add_lrelease(LogDoctor
            TS_FILES ${TS_FILES}
            QM_FILES_OUTPUT_VARIABLE QM_FILES
        )
    else()
        qt_add_lrelease(
            TS_FILES ${TS_FILES}
            QM_FILES_OUTPUT_VARIABLE QM_FILES
        )
    endif()

    #install(FILES ${QM_FILES} DESTINATION resources/translations)

endmacro()
