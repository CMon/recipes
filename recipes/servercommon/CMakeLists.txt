set(LIB_NAME servercommon)

include(GNUInstallDirs)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Core CONFIG REQUIRED)

add_library(${LIB_NAME} SHARED
    clientinfocache.cpp
    clientinfocache.h
)
target_link_libraries(${LIB_NAME}
    Qt5::Core
)
include_directories(
    ${CMAKE_SOURCE_DIR}/modules/rpclib/
    ${CMAKE_SOURCE_DIR}
)

set_target_properties(${LIB_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)
install(TARGETS ${LIB_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
