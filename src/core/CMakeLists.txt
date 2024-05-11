set(SOURCES
    rsignals_slots.c rsignals_slots.h
)

# Find pkg-config
find_package(Threads REQUIRED)


add_library(radiumcore SHARED ${SOURCES})
target_include_directories(radiumcore PUBLIC ${PROJECT_SOURCE_DIR}/include/RadiumCore)
target_link_libraries(radiumcore PRIVATE Threads::Threads)
