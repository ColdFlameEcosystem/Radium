cmake_minimum_required(VERSION 3.5.0)
project(radium VERSION 0.1.0 LANGUAGES C)

# Radium Library
add_subdirectory(src)

# Tests
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
