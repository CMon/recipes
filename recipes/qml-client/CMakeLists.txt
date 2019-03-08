set(TARGET_NAME qml-client)
include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5WebSockets CONFIG REQUIRED)
find_package(Qt5Qml CONFIG REQUIRED)
find_package(Qt5Quick CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

qt5_add_resources(QT_RESOURCES
    qml.qrc
    images.qrc
)

set(SOURCES
    main.cpp
)

add_executable(${TARGET_NAME} ${SOURCES} ${QT_RESOURCES})

target_link_libraries(${TARGET_NAME}
    Qt5::Core
    Qt5::WebSockets
    Qt5::Qml
    Qt5::Quick
    Qt5::Widgets
    rpclib-client
    common
    clientcommon
)

include_directories(
    ${CMAKE_SOURCE_DIR}/modules/rpclib/
    ${CMAKE_SOURCE_DIR}
)
