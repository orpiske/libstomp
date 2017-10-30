find_path(GRU_INCLUDE_DIR common/gru_base.h
        PATH_SUFFIXES gru-0
        HINTS ${GRU_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
find_library(GRU_LIB NAMES gru-0
        HINTS ${GRU_DIR}/${CMAKE_INSTALL_LIBDIR})

message(STATUS "GRU headers found at ${GRU_INCLUDE_DIR}")
message(STATUS "GRU library found at ${GRU_LIB}")


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