include(FetchContent)
cmake_minimum_required(VERSION 3.26.4)
project(KodiakSpoofer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -s")

add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0)

add_executable(KodiakSpoofer main.cpp
        Util/Logger.h
        Util/Terminal.h
        Util/Game.h
        Util/Game.h
        Util/_UUID.h
)