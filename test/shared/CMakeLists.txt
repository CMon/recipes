set(LIB_NAME test_shared)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Sql CONFIG REQUIRED)
find_package(Qt5Test CONFIG REQUIRED)
find_package(Qt5WebSockets CONFIG REQUIRED)

add_library(${LIB_NAME} SHARED
    testserver.cpp
    testserver.h
)

qt5_add_resources(QT_RESOURCES
    ../database.qrc
)

target_link_libraries(${LIB_NAME}
    common
    database
    Qt5::Core
    Qt5::Sql
    Qt5::Test
    Qt5::WebSockets
    rpclib-client
    rpclib-server
    servercommon
    services
)

include_directories(
    ${CMAKE_SOURCE_DIR}/modules/rpclib/
    ${CMAKE_SOURCE_DIR}/test/
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
