link_libraries(
        ${GRU_LIB}
)

add_library(
        litestomp-${LITESTOMP_VERSION_MAJOR} SHARED
        common/stomp_status.c
        common/ls_connection.c
        common/ls_io.c
        stomp_io.c
        stomp_engine.c

        stomp_message.c
        stomp_ex_properties.c
        stomp_messenger.c
)

target_link_libraries(
        litestomp-${LITESTOMP_VERSION_MAJOR}
        ${APR_LIB}
        ${APR_UTIL_LIB}
        g
)

install(TARGETS
        litestomp-${LITESTOMP_VERSION_MAJOR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

install(DIRECTORY .
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${LITESTOMP_VERSION_MAJOR}
        FILES_MATCHING PATTERN "*.h")

