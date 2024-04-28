
function(logdoc_add_install_directives)

    set(INSTALLATION_STUFF_DIR ${CMAKE_SOURCE_DIR}/../installation_stuff)

    # Linux/Flatpak
    if(UNIX AND NOT APPLE)
        install(TARGETS
            LogDoctor
                RUNTIME DESTINATION /usr/bin
                CONFIGURATIONS Release
                PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ
                RENAME logdoctor
        )

        if(BSD)
            install(FILES
                ${INSTALLATION_STUFF_DIR}/LogDoctor.desktop
                    DESTINATION /usr/local/share/applications
            )
        else()
            install(FILES
                ${INSTALLATION_STUFF_DIR}/LogDoctor.desktop
                    DESTINATION /usr/share/applications
            )
        endif()

        install(DIRECTORY
            DESTINATION /usr/share/LogDoctor
        )
        install(FILES
            ${INSTALLATION_STUFF_DIR}/LogDoctor.svg
                DESTINATION /usr/share/LogDoctor
        )
        install(DIRECTORY
            ${INSTALLATION_STUFF_DIR}/logdocdata/help
                DESTINATION /usr/share/LogDoctor
        )
        install(DIRECTORY
            ${INSTALLATION_STUFF_DIR}/logdocdata/licenses
                DESTINATION /usr/share/LogDoctor
        )
    endif()

endfunction()
