cmake_minimum_required(VERSION 3.1)
project(Recipes VERSION 1.0 DESCRIPTION "A Server client app to store recipes")

option(BUILD_CLIENT "build client" ON)
option(BUILD_SERVER "build server" ON)
option(BUILD_TEST "build tests" OFF)
option(CREATE_COVERAGE "run tests and create coverage output" OFF)

if (CREATE_COVERAGE)
    set(BUILD_TEST ON)
    include("${CMAKE_SOURCE_DIR}/cmake/modules/coverage-build.cmake")
endif (CREATE_COVERAGE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=0") # do not shorten compile errors (of cereal)

add_subdirectory(recipes)
add_subdirectory(modules/rpclib/rpclib)

if (BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif(BUILD_TEST)

set(DB_FILES
    doc/structure.md
    database/db_scheme.sql
    database/test_data.sql
)

add_custom_target(databaseFiles SOURCES ${DB_FILES})
