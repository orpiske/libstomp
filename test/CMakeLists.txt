project(ls-tests)

SET(LITESTOMP_TEST_BROKER "stomp://localhost:61613" CACHE STRING "Hostname or IP a test Stomp broker to use")

if (WIN32)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/${CMAKE_INSTALL_LIBDIR})
else (WIN32)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/tests/${CMAKE_INSTALL_BINDIR})
endif (WIN32)


add_executable(
	ls_connection_test
	common/ls_connection_test.c
)

target_link_libraries(
	ls_connection_test
	litestomp-${LITESTOMP_VERSION_MAJOR}
)

add_test(
	NAME ls_connection_test
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
	COMMAND ls_connection_test
)