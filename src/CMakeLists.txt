# Application
include(GNUInstallDirs)
set(DTK_QML_APP_PLUGIN_PATH ${CMAKE_INSTALL_LIBDIR}/dtkdeclarative/qml-app)
add_definitions(-DAPP_PLUGIN_PATH="${DTK_QML_APP_PLUGIN_PATH}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")
    add_definitions(-DLOCALLIBPATH="${LIBRARY_OUTPUT_PATH}")
endif()

add_subdirectory(api)
add_subdirectory(main)
add_subdirectory(preloadplugin)
add_subdirectory(maincomponentplugin)

# 安装 daemon dbus 服务配置
configure_file("dbusservice/com.deepin.Home.Daemon.service.in" "dbusservice/com.deepin.Home.Daemon.service")
install(FILES "${CMAKE_BINARY_DIR}/src/dbusservice/com.deepin.Home.Daemon.service" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/services)
# 安装 icons
install(FILES "maincomponentplugin/images/deepin-home.svg" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
install(FILES "maincomponentplugin/images/deepin-home.svg" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/locolor/scalable/apps)
# 安装 desktop
install(FILES "../deepin-home.desktop" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES "../deepin-home.desktop" DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/skel/Desktop)
# 安装 daemon 的 desktop 文件到全局自启动目录，desktop名字使用APP_NAME，以避免AM将daemon和client识别为两个应用
install(FILES "../deepin-home-daemon.desktop" DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/xdg/autostart RENAME deepin-home.desktop)
