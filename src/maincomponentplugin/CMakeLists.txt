set(MAINCOMPONENT_LIB_NAME dtkqml-${CMAKE_PROJECT_NAME})
set(MAINCOMPONENNT_SRC
    maincomponentplugin.h
    maincomponentplugin.cpp
    worker.h
    worker.cpp
    network.h
    network.cpp
)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt5QuickCompiler REQUIRED)
find_package(Qt5DBus CONFIG REQUIRED)

set_source_files_properties(../dbusservice/com.deepin.Home.Daemon.xml PROPERTIES CLASSNAME HomeDaemonProxy)
qt5_add_dbus_interface(MAINCOMPONENNT_SRC ../dbusservice/com.deepin.Home.Daemon.xml homeDaemonProxy)

qtquick_compiler_add_resources(MAINCOMPONENT_RCC_SOURCES maincomponent.qrc)
include_directories(src/maincomponentplugin)
add_library(${MAINCOMPONENT_LIB_NAME} SHARED
    ${MAINCOMPONENNT_SRC}
    ${MAINCOMPONENT_RCC_SOURCES}
)
target_include_directories(${MAINCOMPONENT_LIB_NAME}
    PUBLIC
    ${DTKDECLARATIVE_INCLUDE_DIR}
)
target_link_libraries(${MAINCOMPONENT_LIB_NAME}
    Qt5::Quick
    Qt5::Widgets
    Qt5::DBus
    ${DtkDeclarative_LIBRARIES}
)
install(TARGETS ${MAINCOMPONENT_LIB_NAME} DESTINATION ${DTK_QML_APP_PLUGIN_PATH})


