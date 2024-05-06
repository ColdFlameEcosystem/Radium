# FindWaylandScanner.cmake
#
# This module finds the wayland-scanner executable and provides commands to generate
# Wayland client and server protocol files.

# Find the wayland-scanner executable
find_program(WAYLAND_SCANNER_EXECUTABLE NAMES wayland-scanner DOC "Path to the wayland-scanner executable")

# Handle the result
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WaylandScanner
    FOUND_VAR WAYLANDSCANNER_FOUND
    REQUIRED_VARS WAYLAND_SCANNER_EXECUTABLE)

# Mark the wayland-scanner executable as advanced
mark_as_advanced(WAYLAND_SCANNER_EXECUTABLE)

# Function to add Wayland client protocol files
function(add_wayland_client_protocol target xmlfile basename)
    set(client_header "${PROJECT_SOURCE_DIR}/${basename}-clients-protocol.h")
    set(client_code "${CMAKE_CURRENT_BINARY_DIR}/${basename}-protocol.c")
    set_source_files_properties(${client_header} GENERATED)
    set_source_files_properties(${client_code} GENERATED)
    add_custom_command(
        OUTPUT ${client_header} ${client_code}
        COMMAND ${WAYLAND_SCANNER_EXECUTABLE} client-header < ${xmlfile} > ${client_header}
        COMMAND ${WAYLAND_SCANNER_EXECUTABLE} private-code < ${xmlfile} > ${client_code}
        DEPENDS ${xmlfile}
        COMMENT "Generating Wayland client protocol files"
    )
    target_sources(${target} PRIVATE ${client_header} ${client_code})
endfunction()

# Function to add Wayland server protocol files
function(add_wayland_server_protocol target xmlfile basename)
    set(server_header "${CMAKE_CURRENT_BINARY_DIR}/wayland-${basename}-server-protocol.h")
    set(server_code "${CMAKE_CURRENT_BINARY_DIR}/wayland-${basename}-protocol.c")
    set_source_files_properties(${server_header} GENERATED)
    set_source_files_properties(${server_code} GENERATED)
    add_custom_command(
        OUTPUT ${server_header} ${server_code}
        COMMAND ${WAYLAND_SCANNER_EXECUTABLE} server-header < ${xmlfile} > ${server_header}
        COMMAND ${WAYLAND_SCANNER_EXECUTABLE} private-code < ${xmlfile} > ${server_code}
        DEPENDS ${xmlfile}
        COMMENT "Generating Wayland server protocol files"
    )
    target_sources(${target} PRIVATE ${server_header} ${server_code})
endfunction()
