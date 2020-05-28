cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(jsonreadercpp)

include_directories(src)

file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(jsonreader-cli ${SOURCES})

set_target_properties(jsonreader-cli PROPERTIES 
                        CXX_STANDARD 17
                        CXX_EXTENSIONS OFF)

#set(CMAKE_BUILD_TYPE Debug)