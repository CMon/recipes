set(TARGET_NAME console-client)
include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5WebSockets CONFIG REQUIRED)

set(SOURCES
    console.cpp
    console.h
    consoleclient.cpp
    consoleclient.h
    main.cpp
    remote.cpp
    remote.h
)

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME} 
    Qt5::Core 
    Qt5::WebSockets
    rpclib-client
    common
)

include_directories(
    ${CMAKE_SOURCE_DIR}/modules/rpclib/
    ${CMAKE_SOURCE_DIR}
)
