cmake_minimum_required(VERSION 3.5)
project(libutils)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(COMMON_COMPILE_OPTIONS
  -Wall
  $<$<CONFIG:RELEASE>:-Werror>
)

option(CC_THREAD_SAFE "Enable/Disable Threadsafe Contructor Cleanup" ON)
if(CC_THREAD_SAFE)
  add_definitions(-DUTILS_CC_THREAD_SAFE)
endif(CC_THREAD_SAFE)

include_directories(utils PUBLIC inc/)

set(UTILS_HEADERS
  inc/utils_am.h
  inc/utils_cc.h
  inc/utils_manager.h
  inc/utils_table.h
  inc/utils_tuple.h
  inc/utils_factory.h
)

set(UTILS_SRC
  src/utils_am.cpp
  src/utils_cc.cpp
)

enable_testing()
add_subdirectory(test)

add_library(utils SHARED ${UTILS_SRC} ${UTILS_HEADERS})
