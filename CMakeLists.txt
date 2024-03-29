cmake_minimum_required(VERSION 3.5)

project(akd VERSION 2.3.2 LANGUAGES CXX)

set(PROJECT_LABEL "Advanced Keyboard Daemon")
set(PROJECT_DESCRIPTION "a keyboard userspace daemon")
set(PROJECT_HOMEPAGE_URL https://github.com/Shatur95/akd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(X11 REQUIRED COMPONENTS xkbfile)
find_package(Boost REQUIRED COMPONENTS program_options)

configure_file(src/cmake.h.in cmake.h)
configure_file(man/${PROJECT_NAME}.1.in man1/${PROJECT_NAME}.1)

add_executable(${PROJECT_NAME}
    src/keyboarddaemon.cpp
    src/keyboardsymbols.cpp
    src/layout.cpp
    src/main.cpp
    src/parameters.cpp
    src/shortcut.cpp
    src/x11deleters.cpp
)

target_link_libraries(${PROJECT_NAME} Boost::program_options X11::xkbfile)

install(TARGETS ${PROJECT_NAME})
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man1 TYPE MAN)
