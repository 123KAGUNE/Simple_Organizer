cmake_minimum_required(VERSION 3.16)
project(simple_organizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

add_executable(simple_organizer
        main.cpp
        organizer.cpp
        mainwindow.cpp
)

target_link_libraries(simple_organizer PRIVATE Qt6::Core Qt6::Widgets)