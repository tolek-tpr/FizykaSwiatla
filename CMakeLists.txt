#cmake_minimum_required(VERSION 3.20)
#project(TrafficController LANGUAGES CXX)
#
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
##find_package(PkgConfig REQUIRED)
##pkg_check_modules(SERIALPORT REQUIRED IMPORTED_TARGET libserialport)
#
#add_library(libserialport SHARED IMPORTED)
#
#set_target_properties(libserialport PROPERTIES
#        IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/lib/lib/libserialport-0.a"
#        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/bin/libserialport-0.dll"
#        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/lib/include"
#)
#
#target_link_libraries(TrafficController PRIVATE libserialport)
#
#add_executable(TrafficController
#        src/main.cpp
#        src/SerialPort.cpp
#        src/SerialPort.cpp
#        include/SerialPort.h
#        include/Intersection.h
#        src/Intersection.cpp
#        include/Packet.h
#)
#
#target_include_directories(TrafficController PRIVATE include)
#
#target_link_libraries(TrafficController PRIVATE
#        PkgConfig::SERIALPORT
#)

cmake_minimum_required(VERSION 3.20)
project(TrafficController LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(libserialport SHARED IMPORTED)

set_target_properties(libserialport PROPERTIES
        IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/lib/lib/libserialport.dll.a"
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/bin/libserialport-0.dll"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/lib/include"
)

add_executable(TrafficController
        src/main.cpp
        src/SerialPort.cpp
        src/Intersection.cpp
        include/SerialPort.h
        include/Intersection.h
        include/Packet.h
)

target_include_directories(TrafficController PRIVATE include)

target_link_libraries(TrafficController PRIVATE libserialport)