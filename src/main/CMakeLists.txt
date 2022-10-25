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
    )

# Find the Qt5Quick library
find_package(Qt5Quick CONFIG REQUIRED)
find_package(Qt5DBus CONFIG REQUIRED)

# Add EXE
add_executable(${APP_NAME} ${SRC})

add_executable(${APP_DAEMON_NAME} daemon.cpp daemon.h)

# 由于 EXE 只是作为启动程序加载 LIB 使用，一般来说，只需链接 ${LIB_NAME} 既可。
target_link_libraries(${APP_NAME}
    Qt5::Quick
    ${DtkDeclarative_LIBRARIES}
    )
target_link_libraries(${APP_DAEMON_NAME}
    Qt5::Core
    Qt5::Network
    Qt5::DBus
    )
set_target_properties(${APP_NAME} PROPERTIES INSTALL_RPATH ${DTK_QML_APP_PLUGIN_PATH})
install(TARGETS ${APP_NAME} DESTINATION ${APP_BIN_INSTALL_DIR})
install(TARGETS ${APP_DAEMON_NAME} DESTINATION ${APP_BIN_INSTALL_DIR})
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../${APP_NAME}.desktop" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../${APP_DAEMON_NAME}.desktop" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../${APP_DAEMON_NAME}.desktop" DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/xdg/autostart)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../dbusservice/com.deepin.Home.Daemon.service" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/services)
