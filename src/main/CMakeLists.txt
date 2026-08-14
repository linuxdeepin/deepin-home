include(GNUInstallDirs)
set(APP_BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin/)
set(APP_NAME ${CMAKE_PROJECT_NAME})
set(APP_DAEMON_NAME ${CMAKE_PROJECT_NAME}-daemon)
set(CMAKE_AUTORCC ON)

add_definitions(-DAPP_NAME="${APP_NAME}")
add_definitions(-DAPP_DAEMON_NAME="${APP_DAEMON_NAME}")
add_definitions(-DAPP_BIN_INSTALL_DIR="${APP_BIN_INSTALL_DIR}")

###! main目录创建出二进制可执行文件，但启动方式由 DAppLoader
###! 中提供, 请不要在 main 中实现额外内容，请将需要实现的部分
###! 移动到 preload 插件和 maincomponent 插件中。

# Set sources
set(SRC
    main.cpp
    )
set(DAEMON_SRC
    daemon.cpp
    homeDaemon.h
    homeDaemon.cpp
    account.h
    account.cpp
    )

# Find the Qt6 libraries
find_package(Qt6 REQUIRED COMPONENTS Quick DBus Widgets Network)
find_package(OpenSSL REQUIRED)

# 通过 dbus 接口的 xml 描述文件，生成调用dbus的 cpp 代码
set_source_files_properties(../dbusservice/org.freedesktop.DBus.Properties.xml PROPERTIES CLASSNAME PropertiesChangedProxy)
set_source_files_properties(../dbusservice/org.freedesktop.DBus.Properties.xml PROPERTIES NO_NAMESPACE ON)
qt6_add_dbus_interface(DAEMON_SRC ../dbusservice/org.freedesktop.DBus.Properties.xml propertiesChangedProxy)

set_source_files_properties(../dbusservice/org.desktopspec.ApplicationManager1.Application.xml PROPERTIES CLASSNAME ApplicationManager1Application)
set_source_files_properties(../dbusservice/org.desktopspec.ApplicationManager1.Application.xml PROPERTIES NO_NAMESPACE ON)
qt6_add_dbus_interface(DAEMON_SRC ../dbusservice/org.desktopspec.ApplicationManager1.Application.xml applicationManager1Application)

set_source_files_properties(../dbusservice/com.deepin.deepinid.xml PROPERTIES CLASSNAME DeepinidDaemonProxy)
set_source_files_properties(../dbusservice/com.deepin.deepinid.xml PROPERTIES NO_NAMESPACE ON)
qt6_add_dbus_interface(DAEMON_SRC ../dbusservice/com.deepin.deepinid.xml deepinidDaemonProxy)

set_source_files_properties(../dbusservice/com.deepin.deepinid.Client.xml PROPERTIES CLASSNAME DeepinidClientProxy)
set_source_files_properties(../dbusservice/com.deepin.deepinid.Client.xml PROPERTIES NO_NAMESPACE ON)
qt6_add_dbus_interface(DAEMON_SRC ../dbusservice/com.deepin.deepinid.Client.xml deepinidClientProxy)

set_source_files_properties(../dbusservice/com.deepin.Home.Daemon.xml PROPERTIES CLASSNAME HomeDaemonProxy)
qt6_add_dbus_interface(SRC ../dbusservice/com.deepin.Home.Daemon.xml homeDaemonProxy)

# 通过 dbus 接口的 xml 描述文件，生成 Adaptor 类，使用 Adaptor 可以选择性的暴露类接口到 dbus
# 可使用 qdbuscpp2xml 生成类的 xml 描述文件，再通过修改 xml 来隐藏部分接口
# daemon 的 xml 描述文件生成： qdbuscpp2xml "src/main/homeDaemon.h" "-o" "src/dbusservice/com.deepin.Home.Daemon.xml"
qt6_add_dbus_adaptor(DAEMON_SRC ../dbusservice/com.deepin.Home.Daemon.xml homeDaemon.h HomeDaemon homeDaemonAdaptor HomeDaemonAdaptor)

# 翻译处理：使用 Qt6 CMake 标准函数 qt6_add_translation 编译 .ts → .qm
# qt6_add_translation 和 qt6_add_resources 需在同一目录作用域，
# 因 add_custom_command(OUTPUT ...) 仅在同目录可见（Makefile 生成器限制）
file(GLOB TS_FILES_APP CONFIGURE_DEPENDS ../../translations/deepin-home/*.ts)
set_source_files_properties(${TS_FILES_APP} PROPERTIES
    OUTPUT_LOCATION "${CMAKE_SOURCE_DIR}/translations/deepin-home")
qt6_add_translation(QM_FILES_APP ${TS_FILES_APP})

file(GLOB TS_FILES_DAEMON CONFIGURE_DEPENDS ../../translations/deepin-home-daemon/*.ts)
set_source_files_properties(${TS_FILES_DAEMON} PROPERTIES
    OUTPUT_LOCATION "${CMAKE_SOURCE_DIR}/translations/deepin-home-daemon")
qt6_add_translation(QM_FILES_DAEMON ${TS_FILES_DAEMON})

# 使用 qt6_add_resources 旧形式（接收 .qrc 文件路径）编译 .qrc → .cpp
# 旧形式通过显式 DEPENDS 链接 qt6_add_translation 的 .qm 生成命令；
# 新形式以 PREFIX 生成 .qrc 路径，app/daemon 的 PREFIX 均为 /resources 会冲突
qt6_add_resources(QRC_CPP_APP ../../translations/deepin-home.qrc)
qt6_add_resources(QRC_CPP_DAEMON ../../translations/deepin-home-daemon.qrc)

list(APPEND SRC ${QRC_CPP_APP})
list(APPEND DAEMON_SRC ${QRC_CPP_DAEMON})

# Add Client EXE
add_executable(${APP_NAME} ${SRC})
# Add Daemon Exec
add_executable(${APP_DAEMON_NAME} ${DAEMON_SRC})

# 由于 EXE 只是作为启动程序加载 LIB 使用，一般来说，只需链接 ${LIB_NAME} 既可。
target_link_libraries(${APP_NAME}
    Qt6::Quick
    Qt6::DBus
    Dtk6::Declarative
    )
target_link_libraries(${APP_DAEMON_NAME}
    Qt6::Core
    Qt6::Network
    Qt6::DBus
    Qt6::Widgets
    OpenSSL::SSL
    API
    )

set_target_properties(${APP_NAME} PROPERTIES INSTALL_RPATH ${DTK_QML_APP_PLUGIN_PATH})
# 安装 deepin-home
install(TARGETS ${APP_NAME} DESTINATION ${APP_BIN_INSTALL_DIR})
# 安装 deepin-home-daemon
install(TARGETS ${APP_DAEMON_NAME} DESTINATION ${APP_BIN_INSTALL_DIR})
