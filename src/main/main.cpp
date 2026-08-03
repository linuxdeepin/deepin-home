// SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include <DAppLoader>
#include <QDBusConnection>
#include <QDir>
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
    // Avoid gray menu background, see https://github.com/linuxdeepin/dtk/issues/70
    qputenv("D_POPUP_MODE", "embed");
    // DTK6 changed the PLUGIN_PATH; pass the legacy path so DAppLoader can
    // fall back to it if the new path is unavailable.
    auto oldPath = QDir(DTK_QML_APP_PLUGIN_PATH);
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
    return appLoader.exec(argc, argv);
}
