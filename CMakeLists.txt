cmake_minimum_required(VERSION 3.14)

project(Threads LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

foreach(_target lab1 lab2 lab3 lab4 lab5)
    add_executable(${_target} "${_target}.cpp")

    if (_target STREQUAL "lab3")
        target_sources(${_target} PRIVATE
            Queue.h
        )
    endif()

    if (_target STREQUAL "lab4")
        target_sources(${_target} PRIVATE
            Barrier.h Barrier.cpp
        )
    endif()

    if (_target STREQUAL "lab5")
        target_link_libraries(${_target}
            OpenMP::OpenMP_CXX
        )
    endif()

    target_link_libraries(${_target}
        Qt${QT_VERSION_MAJOR}::Core
    )
endforeach()
