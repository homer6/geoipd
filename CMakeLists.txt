cmake_minimum_required(VERSION "2.8.2")

project(geoipd CXX C)
add_subdirectory(src)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "GCC detected, adding compile flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
endif(CMAKE_COMPILER_IS_GNUCXX)
