include(${PROJECT_SOURCE_DIR}/cmake_modules/FindWaylandScanner.cmake)

add_library(radium-wayland-backend
    client.c
    client.h
    sharedMemory.h
    sharedMemory.c)
add_wayland_client_protocol(radium-wayland-backend
    ${PROJECT_SOURCE_DIR}/deps/wayland-protocols/stable/xdg-shell/xdg-shell.xml
    xdg-shell)
target_include_directories(radium-wayland-backend PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(radium-wayland-backend PUBLIC wayland-client)
