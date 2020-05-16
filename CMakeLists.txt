cmake_minimum_required(VERSION 3.5)

project(akd VERSION 2.1.0 LANGUAGES CXX DESCRIPTION "Advanced Keyboard Daemon")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(X11 COMPONENTS xkbfile REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

configure_file(src/cmake.h.in cmake.h)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/keyboarddaemon.cpp
    src/shortcut.cpp
    src/layout.cpp
    src/x11deleters.cpp
    src/parameters.cpp
)

target_link_libraries(${PROJECT_NAME} Boost::program_options X11::xkbfile)

install(TARGETS ${PROJECT_NAME})
