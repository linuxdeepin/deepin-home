set(MAINCOMPONENT_LIB_NAME ${CMAKE_PROJECT_NAME}-main)
set(MAINCOMPONENNT_SRC
    maincomponentplugin.h
    maincomponentplugin.cpp
    worker.h
    worker.cpp
    network.h
    network.cpp
    apiproxy.h
    apiproxy.cpp
)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick DBus Concurrent)
find_package(OpenSSL REQUIRED)

set_source_files_properties(../dbusservice/com.deepin.Home.Daemon.xml PROPERTIES CLASSNAME HomeDaemonProxy)
qt6_add_dbus_interface(MAINCOMPONENNT_SRC ../dbusservice/com.deepin.Home.Daemon.xml homeDaemonProxy)

include_directories(src/maincomponentplugin)
add_library(${MAINCOMPONENT_LIB_NAME} SHARED
    ${MAINCOMPONENNT_SRC}
    maincomponent.qrc
)
target_link_libraries(${MAINCOMPONENT_LIB_NAME}
    Qt6::Quick
    Qt6::Widgets
    Qt6::DBus
    Qt6::Concurrent
    OpenSSL::SSL
    Dtk6::Declarative
    API
)
install(TARGETS ${MAINCOMPONENT_LIB_NAME} DESTINATION ${DTK_QML_APP_PLUGIN_PATH})
