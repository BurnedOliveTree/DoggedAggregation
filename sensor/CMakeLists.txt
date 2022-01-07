project(Sensor)
set(G++_COVERAGE_COMPILE_FLAG = "-pthread")
set(CMAKE_CXX_STANDARD 17)

add_library(
    Everything
    lib/Host.h
    lib/Host.cpp
    lib/DataSerializer.h
    lib/DataSerializer.cpp
    lib/Socket.h
    lib/Socket.cpp
    lib/SocketUDP.h
    lib/SocketInterface.h
    lib/SocketUDP.cpp
        lib/Utils.h
    lib/Utils.cpp
        lib/Timer.h lib/Timer.cpp)

add_executable(sensor sensor.cpp)
target_link_libraries(sensor PUBLIC Everything)
