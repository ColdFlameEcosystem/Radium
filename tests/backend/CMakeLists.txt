set(SOURCES
    waylandwindow.c)

add_executable(wayland-backend-window-test
    ${SOURCES})
target_include_directories(wayland-backend-window-test
    PUBLIC ${PROJECT_SOURCE_DIR}/include/Radium)
target_link_libraries(wayland-backend-window-test
    radiumbackend)
