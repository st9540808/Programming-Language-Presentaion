cmake_minimum_required(VERSION 3.4)

project(cpp)

file(GLOB SOURCE *.cpp)
file(GLOB HEADER *.h)

add_executable(cpp ${SOURCE} ${HEADER})
