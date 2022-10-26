// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "./home-daemon.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("deepin");
    QCoreApplication::setOrganizationDomain("deepin.org");
    QCoreApplication::setApplicationName(APP_NAME);
    QApplication app(argc, argv);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.registerService("com.deepin.Home.Daemon")) {
        qDebug() << "DBus Error" << dbus.lastError().message();
        return -1;
    }
    HomeDaemon daemon;
    dbus.registerObject("/com/deepin/Home/Daemon",
                        &daemon,
                        QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);
    daemon.start();
    qDebug() << APP_NAME << "start exec";
    return app.exec();
}
