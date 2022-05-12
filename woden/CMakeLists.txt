cmake_minimum_required(VERSION 3.16)
project(woden C)

set(CMAKE_C_STANDARD 11)
set(WODEN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(WODEN_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(WODEN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB WODEN_SOURCES "${WODEN_SOURCE_DIR}/*.c")
file(GLOB WODEN_TESTS "${WODEN_TEST_DIR}/*.c")

list(REMOVE_ITEM WODEN_SOURCES "${WODEN_SOURCE_DIR}/main.c")
list(REMOVE_ITEM WODEN_TESTS "${WODEN_TEST_DIR}/main.c")

find_package(PkgConfig)
pkg_check_modules(GLIB glib-2.0)

include_directories(${WODEN_INCLUDE_DIR} ${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})

add_definitions(${GLIB_CFLAGS_OTHER})

add_executable(woden ${WODEN_SOURCE_DIR}/main.c ${WODEN_SOURCES})
add_executable(test ${WODEN_TEST_DIR}/main.c ${WODEN_TESTS} ${WODEN_SOURCES})

target_link_libraries(test ${GLIB_LIBRARIES})

install(TARGETS woden DESTINATION bin)
