set(TARGET_NAME database_test)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Sql CONFIG REQUIRED)
find_package(Qt5Test CONFIG REQUIRED)
find_package(Qt5WebSockets CONFIG REQUIRED)

qt5_add_resources(QT_RESOURCES
    ../../database.qrc
)

set(SOURCES
    database_test.cpp
    database_test.h
)

add_executable(${TARGET_NAME} ${SOURCES} ${QT_RESOURCES})

target_link_libraries(${TARGET_NAME}
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
    test_shared
)

include_directories(
    ${CMAKE_SOURCE_DIR}/modules/rpclib/
    ${CMAKE_SOURCE_DIR}/test/
    ${CMAKE_SOURCE_DIR}
)
