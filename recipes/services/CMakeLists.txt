set(LIB_NAME services)

include(GNUInstallDirs)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5WebSockets CONFIG REQUIRED)

set(PUBLIC_HEADER
    logCategory.h
    recipeservice.h
    userservice.h
)

add_library(${LIB_NAME} SHARED ${PUBLIC_HEADER}
    recipeservice.cpp
    userservice.cpp
)
target_link_libraries(${LIB_NAME}
    Qt5::Core
    Qt5::WebSockets
    database
    rpclib-common
    servercommon
)
include_directories(
    ${CMAKE_SOURCE_DIR}/modules/rpclib/
    ${CMAKE_SOURCE_DIR}
)

set_target_properties(${LIB_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER ${PUBLIC_HEADER}
)
install(TARGETS ${LIB_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
