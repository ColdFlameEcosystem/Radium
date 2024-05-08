set(SOURCES
    r_signals_slots.c
)

# For the sake of qtcreator
set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/RadiumCore/r_signals_slots.h
)

# Find pkg-config
find_package(Threads REQUIRED)


add_library(radiumcore SHARED ${SOURCES} ${HEADERS})
target_include_directories(radiumcore PUBLIC ${PROJECT_SOURCE_DIR}/include/RadiumCore)
target_link_libraries(radiumcore PRIVATE Threads::Threads)
