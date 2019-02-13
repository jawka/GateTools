cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(prm)

# CFLAGS:

add_definitions(-std=c++11 -Wall -Wunused-parameter)

# Searching for dependencies:

list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT 6 REQUIRED COMPONENTS Tree)
if(ROOT_USE_FILE)
  include(${ROOT_USE_FILE})
endif()
message(STATUS "ROOT version: ${ROOT_VERSION}")
add_definitions(${ROOT_DEFINITIONS})

find_package(Boost 1.65.1 REQUIRED COMPONENTS program_options)
add_definitions(${Boost_DEFINITIONS})

include_directories(${ROOT_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

# Building:

add_executable(prm prm.cc ${sources} ${headers})

# Linking:

target_link_libraries(prm ${ROOT_LIBRARIES} ${Boost_LIBRARIES})

# Installing:

set_target_properties(prm PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
install (TARGETS prm DESTINATION bin)
