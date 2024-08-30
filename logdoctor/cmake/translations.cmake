
set(TS_FILES
    translations/LogDoctor_ar_AR.ts
    translations/LogDoctor_az_AZ.ts
    translations/LogDoctor_bg_BG.ts
    translations/LogDoctor_bn_BN.ts
    translations/LogDoctor_ca_SP.ts
    translations/LogDoctor_cs_CZ.ts
    translations/LogDoctor_da_DA.ts
    translations/LogDoctor_de_GE.ts
    translations/LogDoctor_el_GR.ts
    translations/LogDoctor_en_GB.ts
    translations/LogDoctor_eo_EO.ts
    translations/LogDoctor_es_SP.ts
    translations/LogDoctor_et_ES.ts
    translations/LogDoctor_fa_PE.ts
    translations/LogDoctor_fi_FI.ts
    translations/LogDoctor_fr_FR.ts
    translations/LogDoctor_ga_IR.ts
    translations/LogDoctor_he_HE.ts
    translations/LogDoctor_hi_IN.ts
    translations/LogDoctor_hu_HU.ts
    translations/LogDoctor_id_ID.ts
    translations/LogDoctor_it_IT.ts
    translations/LogDoctor_ja_JA.ts
    translations/LogDoctor_ko_KO.ts
    translations/LogDoctor_lt_LI.ts
    translations/LogDoctor_lv_LA.ts
    translations/LogDoctor_ms_MA.ts
    translations/LogDoctor_nb_NB.ts
    translations/LogDoctor_nl_NL.ts
    translations/LogDoctor_pl_PL.ts
    translations/LogDoctor_pt_BR.ts
    translations/LogDoctor_pt_PT.ts
    translations/LogDoctor_ro_RO.ts
    translations/LogDoctor_ru_RU.ts
    translations/LogDoctor_sk_SK.ts
    translations/LogDoctor_sl_SL.ts
    translations/LogDoctor_sq_AL.ts
    translations/LogDoctor_sv_SV.ts
    translations/LogDoctor_th_TH.ts
    translations/LogDoctor_tl_PH.ts
    translations/LogDoctor_tr_TR.ts
    translations/LogDoctor_uk_UK.ts
    translations/LogDoctor_ur_PA.ts
    translations/LogDoctor_zh_CN.ts
    translations/LogDoctor_zt_CN.ts
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
