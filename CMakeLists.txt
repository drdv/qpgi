cmake_minimum_required (VERSION 3.0)
project (qpgi CXX)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra")
set (CMAKE_VERBOSE_MAKEFILE OFF)

option(QPGI_BUILD_TESTS "Build tests" ON)
option(QPGI_BUILD_NOTES "Build notes" ON)

#add_definitions(-DQPGI_ENABLE_LOGGER)

find_package(Eigen3 REQUIRED)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})
include_directories("${PROJECT_SOURCE_DIR}/include")

if (QPGI_BUILD_NOTES)
  add_subdirectory(notes)
endif (QPGI_BUILD_NOTES)

if (QPGI_BUILD_TESTS)
  enable_testing()
  include(qpgi_add_test)
  add_subdirectory("${PROJECT_SOURCE_DIR}/tests")
endif(QPGI_BUILD_TESTS)
