cmake_minimum_required(VERSION 3.11)
project(csgo-horker VERSION 0.1.0 LANGUAGES CXX)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

find_package (Threads REQUIRED)
add_subdirectory(src)
