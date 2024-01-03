include(GNUInstallDirs)
set(APP_BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin/)
set(APP_NAME ${CMAKE_PROJECT_NAME})
set(APP_DAEMON_NAME ${CMAKE_PROJECT_NAME}-daemon)
set(CMAKE_AUTORCC ON)

add_definitions(-DAPP_NAME="${APP_NAME}")
add_definitions(-DAPP_DAEMON_NAME="${APP_DAEMON_NAME}")
add_definitions(-DAPP_BIN_INSTALL_DIR="${APP_BIN_INSTALL_DIR}")

if(${DTK_OLD_VERSION})
  add_definitions(-DDTK_OLD_VERSION)
endif()
###! main目录创建出二进制可执行文件，但启动方式由 DAppLoader
###! 中提供, 请不要在 main 中实现额外内容，请将需要实现的部分
###! 移动到 preload 插件和 maincomponent 插件中。

# Set sources
set(SRC
    main.cpp
    ../../translations/deepin-home.qrc
    )
set(DAEMON_SRC
    daemon.cpp 
    homeDaemon.h
    homeDaemon.cpp
    account.h
    account.cpp
    ../../translations/deepin-home-daemon.qrc
    )

# Find the Qt5Quick library
find_package(Qt5Quick CONFIG REQUIRED)
find_package(Qt5DBus CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

# 通过 dbus 接口的 xml 描述文件，生成调用dbus的 cpp 代码
set_source_files_properties(../dbusservice/org.freedesktop.DBus.Properties.xml PROPERTIES CLASSNAME PropertiesChangedProxy)
set_source_files_properties(../dbusservice/org.freedesktop.DBus.Properties.xml PROPERTIES NO_NAMESPACE ON)
qt5_add_dbus_interface(DAEMON_SRC ../dbusservice/org.freedesktop.DBus.Properties.xml propertiesChangedProxy)

set_source_files_properties(../dbusservice/org.desktopspec.ApplicationManager1.Application.xml PROPERTIES CLASSNAME ApplicationManager1Application)
set_source_files_properties(../dbusservice/org.desktopspec.ApplicationManager1.Application.xml PROPERTIES NO_NAMESPACE ON)
qt5_add_dbus_interface(DAEMON_SRC ../dbusservice/org.desktopspec.ApplicationManager1.Application.xml applicationManager1Application)

set_source_files_properties(../dbusservice/com.deepin.deepinid.xml PROPERTIES CLASSNAME DeepinidDaemonProxy)
set_source_files_properties(../dbusservice/com.deepin.deepinid.xml PROPERTIES NO_NAMESPACE ON)
qt5_add_dbus_interface(DAEMON_SRC ../dbusservice/com.deepin.deepinid.xml deepinidDaemonProxy)

set_source_files_properties(../dbusservice/com.deepin.deepinid.Client.xml PROPERTIES CLASSNAME DeepinidClientProxy)
set_source_files_properties(../dbusservice/com.deepin.deepinid.Client.xml PROPERTIES NO_NAMESPACE ON)
qt5_add_dbus_interface(DAEMON_SRC ../dbusservice/com.deepin.deepinid.Client.xml deepinidClientProxy)

set_source_files_properties(../dbusservice/com.deepin.Home.Daemon.xml PROPERTIES CLASSNAME HomeDaemonProxy)
qt5_add_dbus_interface(SRC ../dbusservice/com.deepin.Home.Daemon.xml homeDaemonProxy)

# 通过 dbus 接口的 xml 描述文件，生成 Adaptor 类，使用 Adaptor 可以选择性的暴露类接口到 dbus
# 可使用 qdbuscpp2xml 生成类的 xml 描述文件，再通过修改 xml 来隐藏部分接口
# daemon 的 xml 描述文件生成： qdbuscpp2xml "src/main/homeDaemon.h" "-o" "src/dbusservice/com.deepin.Home.Daemon.xml"
qt5_add_dbus_adaptor(DAEMON_SRC ../dbusservice/com.deepin.Home.Daemon.xml homeDaemon.h HomeDaemon homeDaemonAdaptor HomeDaemonAdaptor)

# Add Client EXE
add_executable(${APP_NAME} ${SRC})
# Add Daemon Exec
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
    OpenSSL::SSL
    API
    )

set_target_properties(${APP_NAME} PROPERTIES INSTALL_RPATH ${DTK_QML_APP_PLUGIN_PATH})
# 安装 deepin-home
install(TARGETS ${APP_NAME} DESTINATION ${APP_BIN_INSTALL_DIR})
# 安装 deepin-home-daemon
install(TARGETS ${APP_DAEMON_NAME} DESTINATION ${APP_BIN_INSTALL_DIR})
# 安装 desktop
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../${APP_NAME}.desktop" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../${APP_NAME}.desktop" DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/skel/Desktop)
# 安装 daemon 的 desktop 文件到全局自启动目录，desktop名字使用APP_NAME，以避免将daemon和client识别为两个应用
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../${APP_DAEMON_NAME}.desktop" DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/xdg/autostart RENAME ${APP_NAME}.desktop)
# 安装 daemon dbus 服务配置
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../dbusservice/com.deepin.Home.Daemon.service" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/services)
# 安装 icons
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../maincomponentplugin/images/deepin-home.svg" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../maincomponentplugin/images/deepin-home.svg" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/locolor/scalable/apps)
