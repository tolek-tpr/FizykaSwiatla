cmake_minimum_required(VERSION 3.20)
project(TrafficController LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SERIALPORT REQUIRED IMPORTED_TARGET libserialport)

add_executable(TrafficController
        src/main.cpp
        src/SerialPort.cpp
        src/SerialPort.cpp
        include/SerialPort.h
        include/Intersection.h
        src/Intersection.cpp
        include/Packet.h
)

target_include_directories(TrafficController PRIVATE include)

target_link_libraries(TrafficController PRIVATE
        PkgConfig::SERIALPORT
)