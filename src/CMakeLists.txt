file(GLOB sources *.cc)
add_executable( ../geoipd ./server/main.cc ${sources} )
add_executable( ../geoip ./client/main.cc ${sources} )

find_package( Boost 1.37 COMPONENTS system program_options regex thread REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

target_link_libraries( ../geoipd ${Boost_LIBRARIES} )
target_link_libraries( ../geoip ${Boost_LIBRARIES} )

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "GCC detected, adding compile flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)
