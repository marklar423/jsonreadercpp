cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(jsoncpp)

file(GLOB SOURCES src/*.cc)

add_executable(jsoncpp ${SOURCES})