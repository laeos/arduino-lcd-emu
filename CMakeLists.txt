cmake_minimum_required(VERSION 3.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(lcd-emu)

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

add_compile_options("-std=c++11")

find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(Threads REQUIRED)

include_directories(${SFML_INCLUDE_DIR})
include_directories("${PROJECT_BINARY_DIR}")
include_directories(Adafruit-GFX-Library Arduino)

set(COMMON_SRC Adafruit-GFX-Library/Adafruit_GFX.cpp Arduino/Print.cpp Arduino/shim.cpp)

set(TARGET "lcd-emu-ili9341")
add_executable(${TARGET} lcd-emu-ili9341.cpp ${COMMON_SRC})
target_link_libraries(${TARGET} ${SFML_LIBRARIES})

set(TARGET "lcd-emu-hx8357")
add_executable(${TARGET} lcd-emu-hx8357.cpp ${COMMON_SRC})
target_link_libraries(${TARGET} ${SFML_LIBRARIES})
