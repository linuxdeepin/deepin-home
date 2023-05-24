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
        qDebug() << "DBus Error: register com.deepin.Home";
        return -1;
    }
    DAppLoader appLoader(APP_NAME);
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
