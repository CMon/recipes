set(TARGET_NAME server)
include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5WebSockets CONFIG REQUIRED)
find_package(Qt5Sql CONFIG REQUIRED)

qt5_add_resources(QT_RESOURCES
    ../../database.qrc
)

set(SOURCES
    main.cpp
    server.cpp
    server.h
)

add_executable(${TARGET_NAME} ${SOURCES} ${QT_RESOURCES})

target_link_libraries(${TARGET_NAME}
    Qt5::Core
    Qt5::WebSockets
    Qt5::Sql
    rpclib-server
    common
    database
    servercommon
    services
)

include_directories(
    ${CMAKE_SOURCE_DIR}/modules/rpclib/
    ${CMAKE_SOURCE_DIR}
)
