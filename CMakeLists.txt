cmake_minimum_required(VERSION 3.5)
project(libutils)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(utils PUBLIC inc/)

set(UTILS_HEADERS
  inc/utils_am.h
  inc/utils_cc.h
  inc/utils_manager.h
  inc/utils_table.h
  inc/utils_tuple.h
)

set(UTILS_SRC
  src/utils_am.cpp
)

add_library(utils SHARED ${UTILS_SRC} ${UTILS_HEADERS})
