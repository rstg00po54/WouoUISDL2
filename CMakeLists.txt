cmake_minimum_required(VERSION 3.24)
project(WouoUI)

set(CMAKE_C_COMPILER "cc")
set(CMAKE_CXX_COMPILER "c++")
set(CMAKE_NO_SYSTEM_FROM_IMPORTED YES)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})


file(GLOB SRCS_EMULATOR
        Libs/u8g2/csrc/*.c 
        Libs/u8g2/sys/sdl/common/*.c 
        Libs/u8g2/cppsrc/*.cpp 
        Libs/u8g2/common/*.c
        Libs/Arduino/*.c 
        Libs/Arduino/*.cpp
        Libs/WouoFix.cpp
        *.c 
        *.cpp
        src/Arduino.cpp
        WouoUI-Demo/ui-128_64/*.cpp
        )
include_directories(Libs/u8g2/csrc/)
include_directories(Libs/u8g2/cppsrc/)
include_directories(Libs/Arduino/)
include_directories(Libs/)
include_directories(Libs/u8g2/cppsrc/)
include_directories(Libs/u8g2/common/)
include_directories(SDL2/include/)
include_directories(src/)
include_directories(.)
include_directories(WouoUI-Demo/)
add_definitions(-w)

include_directories(.)

add_executable(WouoUI ${SRCS_EMULATOR})


target_link_libraries(WouoUI SDL2)

