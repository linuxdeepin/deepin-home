// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include <DAppLoader>
#include <QDBusConnection>
#include <QQmlApplicationEngine>
#include "../base/const.h"
#include "homeDaemonProxy.h"

DQUICK_USE_NAMESPACE

void activeMainWindows()
{
    HomeDaemonProxy daemon(DEEPIN_HOME_DAEMON_SERVICE,
                                DEEPIN_HOME_DAEMON_PATH,
                                QDBusConnection::sessionBus());
    daemon.activeMainWindows();
}

int main(int argc, char *argv[])
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.registerService("com.deepin.Home")) {
        activeMainWindows();
        qDebug() << "Register DBus Error" << dbus.lastError().message() << "Process may already be running.";
        return -1;
    }
// 兼容旧版本 DtkDeclarative
#ifdef DTK_OLD_VERSION
    #ifdef LOCALLIBPATH
        DAppLoader appLoader(APP_NAME, LOCALLIBPATH);
    #else
        DAppLoader appLoader(APP_NAME);
    #endif
#else
    // 避免窗口菜单背景显示灰色 https://github.com/linuxdeepin/dtk/issues/70
    // 在v20.9会导致菜单延迟显示，所以只在v23使用
    qputenv("D_POPUP_MODE", "embed");
    // dtk 在新版本修改了 PLUGIN_PATH 的路径
    // 为避免在旧版本 dtk 中无法使用，将旧的 PLUGIN_PATH 传递到 appLoader
    // 如果传递的路径不存在 DAppLoader 内部会 feedback 到新路径，实现了新旧 dtk 兼容
    auto oldPath=QDir(DTK_QML_APP_PLUGIN_PATH);
    oldPath.cd("../plugins");
    DAppLoader appLoader(APP_NAME, oldPath.path());
    appLoader.addPluginPath(oldPath.path());
    #ifdef PLUGINPATH
        appLoader.addPluginPath(PLUGINPATH);
    #endif
    #ifdef APP_PLUGIN_PATH
        appLoader.addPluginPath(APP_PLUGIN_PATH);
    #endif
    #ifdef LOCALLIBPATH
        appLoader.addPluginPath(LOCALLIBPATH);
    #endif
#endif
    return appLoader.exec(argc, argv);
}
