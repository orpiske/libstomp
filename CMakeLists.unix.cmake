find_path(GRU_INCLUDE_DIR common/gru_base.h
        PATH_SUFFIXES gru-0
        HINTS ${GRU_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
find_library(GRU_LIB NAMES gru-0
        HINTS ${GRU_DIR}/${CMAKE_INSTALL_LIBDIR})

message(STATUS "GRU headers found at ${GRU_INCLUDE_DIR}")
message(STATUS "GRU library found at ${GRU_LIB}")