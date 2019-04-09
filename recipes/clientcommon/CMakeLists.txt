set(LIB_NAME clientcommon)

include(GNUInstallDirs)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5WebSockets CONFIG REQUIRED)
find_package(Qt5Sql CONFIG REQUIRED)

add_library(${LIB_NAME} SHARED
    adminserviceinterface.cpp
    adminserviceinterface.h
    clientcontroller.cpp
    clientcontroller.h
    recipesearchmodel.cpp
    recipesearchmodel.h
    userserviceinterface.cpp
    userserviceinterface.h
)
target_link_libraries(${LIB_NAME}
    Qt5::Core
    Qt5::WebSockets
    Qt5::Sql
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
