find_program (FOUND_SCAN_BUILD scan-build)
if (NOT FOUND_SCAN_BUILD)
    message(FATAL_ERROR "could not find scan-build executable which is necessary when adding the target staticCodeAnalysis")
endif ()
message("|-> enabled scan-build")

set(STATIC_ANALYSIS_OUTPUT "/tmp" CACHE STRING "output folder for static code analysis")

add_custom_target(staticCodeAnalysis
    scan-build ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}
    COMMAND scan-build -o ${STATIC_ANALYSIS_OUTPUT} ${CMAKE_MAKE_PROGRAM}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "--> create static code analysis"
)
