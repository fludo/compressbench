cmake_minimum_required(VERSION 2.6)
 
project(Compress)
set(EXECUTABLE_OUTPUT_PATH bin/)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmakefind")
SET(CMAKE_CXX_FLAGS "-march=core2 -O2 -Wall -pedantic -pipe -Wfloat-equal") 
SET(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} ")
add_definitions(-Wall -pedantic)
include_directories (

./src/codecs/*.hpp
./src/*.hpp
  ./src/codecs/huffman/*.hpp

)
 
file(
  GLOB
  source_files
  ./src/*.cpp
  
  ./src/codecs/*.cpp
  
  ./src/codecs/huffman/*.cpp
 src/codeschemes/*.cpp
)
 
add_executable (
  compress
  ${source_files}
)
 
target_link_libraries (
  compress
)
