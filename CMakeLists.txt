project(libstomp)
cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/lib)

set(RUNTIME_DIR "bin")
set(CPP_INCLUDE_DIR "include")

if( CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "64 bits compiler detected")
    set(APP_BUILD_PLATFORM 64)
    set(APP_BUILD_PLATFORM_NAME "x86_64")
    set(CPP_LIBRARY_DIR "lib64")
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "32 bits compiler detected")
    set(APP_BUILD_PLATFORM 32)
    set(APP_BUILD_PLATFORM_NAME "i686")
    set(CPP_LIBRARY_DIR "lib")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)


if (${CMAKE_SYSTEM_NAME} EQUAL "Linux") 
 add_definitions(-DLINUX_BUILD) 
endif (${CMAKE_SYSTEM_NAME} EQUAL "Linux")

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS "-Wall -Wshadow -fdiagnostics-color=auto -g -std=c99" CACHE STRING
		"Flags used by the compiler during all build types." FORCE
	)
endif (CMAKE_COMPILER_IS_GNUCXX)

add_custom_target(documentation
    COMMAND ${CMAKE_COMMAND} -P ../documentation.cmake)

include_directories(src)

add_definitions(-D_GNU_SOURCE -D_POSIX_C_SOURCE)

if (${CMAKE_SYSTEM_NAME} EQUAL "Linux") 
 add_definitions(-DLINUX_BUILD) 
endif (${CMAKE_SYSTEM_NAME} EQUAL "Linux") 

add_library(
	stomp SHARED
        src/stomp_io.c
        src/stomp_engine.c
        src/stomp_status.c
        src/stomp_message.c
        src/stomp_messenger.c 
)

target_link_libraries(
    stomp
    apr-1
    aprutil-1
)

install(TARGETS
        stomp
	LIBRARY DESTINATION ${CPP_LIBRARY_DIR}
)

install(DIRECTORY src/ 
        DESTINATION include/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.h")



add_subdirectory(example)