cmake_minimum_required(VERSION 2.6)

FILE(GLOB SOURCE_FILES "*.c")
FILE(GLOB HEADER_FILES "*.h" "gen/*.h")
SET(SOURCES ${SOURCE_FILES} ${HEADER_FILES})

add_executable(colint ${SOURCE_FILES})
install(TARGETS colint DESTINATION bin)

