project(gate)
cmake_minimum_required(VERSION 3.15)
set(G++_COVERAGE_COMPILE_FLAG = "-pthread")
set(CMAKE_CXX_STANDARD 17)

add_library(
    Everything
    lib/Header.h
    lib/Host.h
    lib/Host.cpp
    lib/Agregator.h
    lib/Agregator.cpp
    lib/DataSerializer.h
    lib/DataSerializer.cpp
    lib/Socket.h
    lib/Socket.cpp
    lib/SocketUDP.h
    lib/SocketInterface.h
    lib/SocketUDP.cpp
    lib/SocketTCP.h
    lib/SocketTCP.cpp
    lib/Timer.h
    lib/Timer.cpp
    lib/Exit.h
    lib/Exit.cpp
    lib/Utils.h
    lib/Utils.cpp
    lib/Gate.h
    lib/Gate.cpp
    lib/SHA256.h
    lib/SHA256.cpp
    lib/AES.h
    lib/AES.cpp
)

add_executable(gate gate.cpp)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(gate PRIVATE Threads::Threads)
target_link_libraries(gate PUBLIC Everything)
