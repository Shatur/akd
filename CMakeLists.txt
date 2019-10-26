cmake_minimum_required(VERSION 3.5)

project(akd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(X11)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/keyboarddaemon.cpp
    )

target_link_libraries(${PROJECT_NAME} X11::X11)
