cmake_minimum_required(VERSION 4.0)
project(unity_reader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(unity_reader
        main.cpp
        scenes_handler.cpp
        hash_128.h
        hash_128.cpp
        scripts_handler.cpp
        scripts_handler.h
        scenes_handler.h
        unity_element.cpp
        unity_element.h
        utility.cpp
        utility.h
)
