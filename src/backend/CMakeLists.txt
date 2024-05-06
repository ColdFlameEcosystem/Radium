list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")

find_package(WaylandScanner REQUIRED)

set(SOURCES
    wayland/platformwindow.c)

add_library(radiumbackend SHARED ${SOURCES})
add_wayland_client_protocol(radiumbackend
    ${PROJECT_SOURCE_DIR}/deps/wayland-protocols/stable/xdg-shell/xdg-shell.h xdg-shell)
