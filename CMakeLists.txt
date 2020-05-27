cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(jsoncpp)

include_directories(src)

file(GLOB_RECURSE  SOURCES "src/*.cc")

add_executable(jsoncpp-cli ${SOURCES})

set_target_properties(jsoncpp-cli PROPERTIES 
                        CXX_STANDARD 17
                        CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)