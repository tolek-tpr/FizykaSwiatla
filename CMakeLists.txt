cmake_minimum_required(VERSION 3.20)
project(TrafficController LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TrafficController
        src/main.cpp
        src/SerialPort.cpp
        src/Intersection.cpp
        include/SerialPort.h
        include/Intersection.h
        include/Packet.h
)

target_include_directories(TrafficController PRIVATE include)

if(WIN32)
    add_library(libserialport SHARED IMPORTED)

    set_target_properties(libserialport PROPERTIES
            IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/lib/lib/libserialport.dll.a"
            IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/bin/libserialport-0.dll"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/lib/include"
    )

    target_link_libraries(TrafficController PRIVATE libserialport)

    add_custom_command(TARGET TrafficController POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/lib/bin/libserialport-0.dll"
            "$<TARGET_FILE_DIR:TrafficController>"
    )

else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SERIALPORT REQUIRED IMPORTED_TARGET libserialport)

    target_link_libraries(TrafficController PRIVATE PkgConfig::SERIALPORT)

endif()

install(TARGETS TrafficController
        RUNTIME DESTINATION .
)

if(WIN32)
    # Install libserialport DLL
    install(FILES
            "${CMAKE_SOURCE_DIR}/lib/bin/libserialport-0.dll"
            DESTINATION .
    )
endif()

# Install example data
install(DIRECTORY data/
        DESTINATION .
)