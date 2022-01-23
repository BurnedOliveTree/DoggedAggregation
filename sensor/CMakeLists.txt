project(Sensor)
cmake_minimum_required(VERSION 3.15)
set(G++_COVERAGE_COMPILE_FLAG = "-pthread")
set(CMAKE_CXX_STANDARD 17)

add_library(
    Everything
        lib/DataSerializer.cpp
        lib/DataSerializer.h
        lib/Document.cpp
        lib/Document.h
        lib/Host.cpp
        lib/Host.h
        lib/Socket.cpp
        lib/Socket.h
        lib/SocketUDP.cpp
        lib/SocketUDP.h
        lib/SocketInterface.h
        lib/Timer.cpp
        lib/Timer.h
        lib/Utils.cpp
        lib/Utils.h)

add_executable(sensor sensor.cpp)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(sensor PRIVATE Threads::Threads)
target_link_libraries(sensor PUBLIC Everything)
