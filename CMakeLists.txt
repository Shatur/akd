cmake_minimum_required(VERSION 3.5)

project(akd VERSION 2.1.0 LANGUAGES CXX DESCRIPTION "Advanced Keyboard Daemon")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(X11)
find_package(Boost COMPONENTS program_options)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/keyboarddaemon.cpp
    src/shortcut.cpp
    src/layout.cpp
    src/x11deleters.cpp
    src/parameters.cpp
    )

configure_file(src/version.h.in version.h)

target_link_libraries(${PROJECT_NAME} Boost::program_options X11::X11 X11::xkbfile)

install(TARGETS ${PROJECT_NAME})
