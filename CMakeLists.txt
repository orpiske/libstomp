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
set(BUILD_WITH_UNIT_TESTS ON CACHE BOOL "Build unit tests")

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

include_directories(src)

add_subdirectory(src/ objs)

add_subdirectory(example)

if (BUILD_WITH_UNIT_TESTS)
	enable_testing()

	include_directories(test)
	add_subdirectory(test)
endif (BUILD_WITH_UNIT_TESTS)
