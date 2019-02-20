set(LIB_NAME common)

include(GNUInstallDirs)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Core CONFIG REQUIRED)

add_library(${LIB_NAME} SHARED
    category.cpp
    category.h
    ingredient.cpp
    ingredient.h
    locale2string.cpp
    locale2string.h
    log.cpp
    log.h
    permission.cpp
    permission.h
    portion.cpp
    portion.h
    recipe.cpp
    recipe.h
    unit.cpp
    unit.h
    user.cpp
    user.h
    util.cpp
    util.h
    waiter.cpp
    waiter.h
)
target_link_libraries(${LIB_NAME}
    Qt5::Core
    rpclib-common
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
