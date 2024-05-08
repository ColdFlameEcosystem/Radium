set(SOURCES
    signal_slots_test.c)

add_executable(signal-slot-test
    ${SOURCES})

target_include_directories(signal-slot-test PUBLIC ${PROJECT_SOURCE_DIR}/include/RadiumCore)
target_link_libraries(signal-slot-test radiumcore)