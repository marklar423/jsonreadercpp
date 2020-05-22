cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(jsoncpp)

file(GLOB SOURCES src/*.cc)

add_executable(jsoncpp ${SOURCES})

set_target_properties(jsoncpp PROPERTIES 
                        CXX_STANDARD 17
                        CXX_EXTENSIONS OFF)
