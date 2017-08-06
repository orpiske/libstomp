project(litestomp)
cmake_minimum_required(VERSION 2.8)

set(LITESTOMP_VERSION 0.1.0)
set(LITESTOMP_VERSION_MAJOR 0)
set(LITESTOMP_VERSION_MINOR 1)
set(LITESTOMP_VERSION_PATCH 0)

include(GNUInstallDirs)
include(CheckLists.cmake)
include(CMakeLists.default.cmake)

if (UNIX)
	message(STATUS "Adding Unix related build rules")
	include(CMakeLists.unix.cmake)
else (UNIX)
	if (WIN32)
		message(STATUS "Adding Windows related build rules")
		include(CMakeLists.windows.cmake)
	endif (WIN32)
endif(UNIX)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/${CMAKE_INSTALL_BINDIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/${CMAKE_INSTALL_LIBDIR})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	# TODO: POSIX_C_SOURCE is, probably, not necessary. Check and fix.
	add_definitions(-DLINUX_BUILD -D_GNU_SOURCE -D_POSIX_C_SOURCE)
else (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	message(STATUS "Compiling for " ${CMAKE_SYSTEM_NAME} "")
	if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
		set(CMAKE_MACOSX_RPATH TRUE)
		add_definitions(-D__OSX__)
	endif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")


if(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug CACHE STRING
      	"Choose the type of build, options are: Debug, RelWithDebInfo or Release."
      	FORCE
	)
endif(NOT CMAKE_BUILD_TYPE)

macro (FailIfNotSet VARIABLE name)
	IF (NOT ${VARIABLE})
		SET(MSG "** A required file, " ${name} ", was not found on your system **")
		MESSAGE(FATAL_ERROR ${MSG})
	ENDIF (NOT ${VARIABLE})
endmacro(FailIfNotSet)

include (CheckIncludeFiles)

CHECK_INCLUDE_FILES(sys/types.h HAVE_SYS_TYPES_H)
FailIfNotSet(HAVE_SYS_TYPES_H sys/types.h)


add_custom_target(documentation
    COMMAND ${CMAKE_COMMAND} -P ../documentation.cmake)


find_path(APR_INCLUDE_DIR apr_general.h
        PATH_SUFFIXES apr-1 apr-1.0)
find_library(APR_LIB NAMES apr-1)

message(STATUS "APR headers found at ${APR_INCLUDE_DIR}")
message(STATUS "APR library found at ${APR_LIB}")

find_path(APR_UTIL_INCLUDE_DIR apu.h
        PATH_SUFFIXES apr-1 apr-1.0)
find_library(APR_UTIL_LIB NAMES aprutil-1)

message(STATUS "APR util headers found at ${APR_UTIL_INCLUDE_DIR}")
message(STATUS "APR util library found at ${APR_UTIL_LIB}")


include_directories(
        ${APR_INCLUDE_DIR}
        ${APR_UTIL_INCLUDE_DIR}
        ${GRU_INCLUDE_DIR}
        src
        src/common
        src/transport)


link_libraries(
        ${GRU_LIB}
)

add_library(
	litestomp-${LITESTOMP_VERSION_MAJOR} SHARED
        src/common/stomp_status.c
        src/transport/ls_transport_plain.c
        src/stomp_io.c
	    src/stomp_engine.c

	src/stomp_message.c
	src/stomp_ex_properties.c
	src/stomp_messenger.c
	)

target_link_libraries(
	litestomp-${LITESTOMP_VERSION_MAJOR}
	${APR_LIB}
	${APR_UTIL_LIB}
)

install(TARGETS
	litestomp-${LITESTOMP_VERSION_MAJOR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY src/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${LITESTOMP_VERSION_MAJOR}
	FILES_MATCHING PATTERN "*.h")



add_subdirectory(example)
