if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message( WARNING "Code coverage results with an optimised (non-Debug) build may be misleading")
endif()

if (NOT DEFINED CODECOV_HTMLOUTPUTDIR)
    set(CODECOV_HTMLOUTPUTDIR coverage_results)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCXX)
    message("|-> building with coverage analysis data")
    find_program(CODECOV_GCOV gcov)
    find_program(CODECOV_LCOV lcov)
    find_program(CODECOV_GENHTML genhtml)
    if (NOT CODECOV_GCOV)
        message(FATAL_ERROR "could not find gcov executable which is necessary when adding the target createTestCoverage")
    endif()
    if (NOT CODECOV_LCOV)
        message(FATAL_ERROR "could not find lcov executable which is necessary when adding the target createTestCoverage")
    endif()
    if (NOT CODECOV_GENHTML)
        message(FATAL_ERROR "could not find genhtml executable which is necessary when adding the target createTestCoverage")
    endif()

    add_definitions(-fprofile-arcs -ftest-coverage)
    link_libraries(gcov)
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} --coverage)

    add_custom_target(createTestCoverage
        rm -f *.info
        COMMAND ${CODECOV_LCOV} --capture --initial --directory ${CMAKE_BINARY_DIR} --output-file app_base.info
        COMMAND ${CMAKE_MAKE_PROGRAM} test
        COMMAND ${CODECOV_LCOV} --capture --directory ${CMAKE_BINARY_DIR} --output-file app_test.info
        COMMAND ${CODECOV_LCOV} --add-tracefile app_base.info --add-tracefile app_test.info --output-file app_total.info
        COMMAND ${CODECOV_LCOV} --remove app_total.info \"/usr/include/*\" \"${CMAKE_BINARY_DIR}/*\" \"/usr/local/*\" \"*/modules/rpclib/*\" --output-file app_cleaned.info
        COMMAND genhtml -o ${CODECOV_HTMLOUTPUTDIR} app_cleaned.info
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "--> create static code analysis"
    )
else()
    message("Unsupported compiler for coverage")
endif()
