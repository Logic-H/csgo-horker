cmake_minimum_required(VERSION 3.2.0)
project(csgo-horker VERSION 0.1.0 LANGUAGES CXX)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package (Threads REQUIRED)
find_package (X11 REQUIRED)
find_package (NVCtrlLib)

add_subdirectory(src)
