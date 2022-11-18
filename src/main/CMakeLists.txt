include(GNUInstallDirs)
set(APP_BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin/)
set(APP_NAME ${CMAKE_PROJECT_NAME})
set(APP_DAEMON_NAME ${CMAKE_PROJECT_NAME}-daemon)
add_definitions(-DAPP_NAME="${APP_NAME}")

###! main目录创建出二进制可执行文件，但启动方式由 DAppLoader
###! 中提供, 请不要在 main 中实现额外内容，请将需要实现的部分
###! 移动到 preload 插件和 maincomponent 插件中。

# Set sources
set(SRC
    main.cpp
    ../../../translations/deepin-home.qrc
    )
set(CMAKE_AUTORCC ON)
set(DAEMON_SRC
    daemon.cpp 
    homeDaemon.h
    homeDaemon.cpp
    api.h
    api.cpp
    account.h
    account.cpp
    ../../../translations/deepin-home-daemon.qrc
    )

# Find the Qt5Quick library
find_package(Qt5Quick CONFIG REQUIRED)
find_package(Qt5DBus CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

# Add EXE
add_executable(${APP_NAME} ${SRC})

set_source_files_properties(../dbusservice/org.freedesktop.DBus.Properties.xml PROPERTIES CLASSNAME PropertiesChangedProxy)
set_source_files_properties(../dbusservice/org.freedesktop.DBus.Properties.xml PROPERTIES NO_NAMESPACE ON)
qt5_add_dbus_interface(DAEMON_SRC ../dbusservice/org.freedesktop.DBus.Properties.xml propertiesChangedProxy)

set_source_files_properties(../dbusservice/com.deepin.deepinid.xml PROPERTIES CLASSNAME DeepinidDaemonProxy)
set_source_files_properties(../dbusservice/com.deepin.deepinid.xml PROPERTIES NO_NAMESPACE ON)
qt5_add_dbus_interface(DAEMON_SRC ../dbusservice/com.deepin.deepinid.xml deepinidDaemonProxy)

set_source_files_properties(../dbusservice/com.deepin.deepinid.Client.xml PROPERTIES CLASSNAME DeepinidClientProxy)
set_source_files_properties(../dbusservice/com.deepin.deepinid.Client.xml PROPERTIES NO_NAMESPACE ON)
qt5_add_dbus_interface(DAEMON_SRC ../dbusservice/com.deepin.deepinid.Client.xml deepinidClientProxy)

qt5_add_dbus_adaptor(DAEMON_SRC ../dbusservice/com.deepin.Home.Daemon.xml homeDaemon.h HomeDaemon homeDaemonAdaptor HomeDaemonAdaptor)
add_executable(${APP_DAEMON_NAME} ${DAEMON_SRC})

# 由于 EXE 只是作为启动程序加载 LIB 使用，一般来说，只需链接 ${LIB_NAME} 既可。
target_link_libraries(${APP_NAME}
    Qt5::Quick
    Qt5::DBus
    ${DtkDeclarative_LIBRARIES}
    )
target_link_libraries(${APP_DAEMON_NAME}
    Qt5::Core
    Qt5::Network
    Qt5::DBus
    Qt5::Widgets
    )

set_target_properties(${APP_NAME} PROPERTIES INSTALL_RPATH ${DTK_QML_APP_PLUGIN_PATH})
install(TARGETS ${APP_NAME} DESTINATION ${APP_BIN_INSTALL_DIR})
install(TARGETS ${APP_DAEMON_NAME} DESTINATION ${APP_BIN_INSTALL_DIR})
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../${APP_NAME}.desktop" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../${APP_DAEMON_NAME}.desktop" DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/xdg/autostart)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../dbusservice/com.deepin.Home.Daemon.service" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/services)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../maincomponentplugin/images/deepin-home.svg" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../maincomponentplugin/images/deepin-home.svg" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/locolor/scalable/apps)
