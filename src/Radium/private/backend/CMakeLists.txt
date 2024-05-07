list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")

find_package(WaylandScanner REQUIRED)

set(SOURCES
    wayland/platformwindow.c)

add_library(radiumbackend SHARED ${SOURCES})
target_include_directories(radiumbackend PUBLIC ${PROJECT_SOURCE_DIR}/include/Radium)
target_include_directories(radiumbackend PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
add_wayland_client_protocol(radiumbackend
    ${PROJECT_SOURCE_DIR}/deps/wayland-protocols/stable/xdg-shell/xdg-shell.xml xdg-shell)
target_link_libraries(radiumbackend wayland-client)
