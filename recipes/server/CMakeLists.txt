set(TARGET_NAME server)
include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5WebSockets CONFIG REQUIRED)
find_package(Qt5Sql CONFIG REQUIRED)

set(SOURCES
    main.cpp
)

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME}
    Qt5::Core
    Qt5::WebSockets
    Qt5::Sql
    rpclib-client
    common
    database
    servercommon
    services
)

include_directories(${CMAKE_SOURCE_DIR})
