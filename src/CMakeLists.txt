cmake_minimum_required(VERSION 2.6)

FILE(GLOB SOURCE_FILES "*.c")
FILE(GLOB HEADER_FILES "*.h")
SET(SOURCES ${SOURCE_FILES} ${HEADER_FILES})

add_executable(col ${SOURCE_FILES})
install(TARGETS col DESTINATION bin)
