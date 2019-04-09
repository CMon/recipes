cmake_minimum_required(VERSION 3.1)
project(Recipes VERSION 1.0 DESCRIPTION "A Server client app to store recipes")

option(OPT_BUILD_CLIENT "build client" ON)
option(OPT_BUILD_SERVER "build server" ON)
option(OPT_BUILD_TEST "build tests" OFF)
option(OPT_CREATE_COVERAGE "run tests and create coverage output" OFF)
option(OPT_SCAN_BUILD "create a target to run the static code analysis through scan-build" OFF)

if (OPT_CREATE_COVERAGE)
    set(OPT_BUILD_TEST ON)
    include("${CMAKE_SOURCE_DIR}/cmake/modules/coverage-build.cmake")
endif (OPT_CREATE_COVERAGE)

if (OPT_SCAN_BUILD)
    include("${CMAKE_SOURCE_DIR}/cmake/modules/scan-build.cmake")
endif(OPT_SCAN_BUILD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=0") # do not shorten compile errors (of cereal)

add_subdirectory(recipes)
add_subdirectory(modules/rpclib/rpclib)

if (OPT_BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif(OPT_BUILD_TEST)

set(DB_FILES
    doc/structure.md
    database/db_scheme.sql
    database/test_data.sql
)

add_custom_target(databaseFiles SOURCES ${DB_FILES})
