cmake_minimum_required(VERSION 3.5)

project(akd VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(X11)
find_package(Boost COMPONENTS program_options)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/keyboarddaemon.cpp
    src/shortcut.cpp
    src/layout.cpp
    )

target_link_libraries(${PROJECT_NAME} Boost::program_options X11::X11)

install(TARGETS ${PROJECT_NAME})
