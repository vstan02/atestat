cmake_minimum_required(VERSION 3.16)
project(wcode C)

set(CMAKE_C_STANDARD 11)
set(WCODE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB WCODE_SOURCES "${WCODE_SOURCE_DIR}/*.c")
list(REMOVE_ITEM WCODE_SOURCES "${WCODE_SOURCE_DIR}/main.c")

add_executable(wcode ${WCODE_SOURCE_DIR}/main.c ${WCODE_SOURCES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GTK_SW3 REQUIRED gtksourceview-3.0)

include_directories(${WCODE_INCLUDE_DIR} ${GTK3_INCLUDE_DIRS} ${GTK_SW3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS} ${GTK_SW3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} ${GTK_SW3_CFLAGS_OTHER})

target_link_libraries(wcode ${GTK3_LIBRARIES} ${GTK_SW3_LIBRARIES})

install(TARGETS wcode DESTINATION bin)
