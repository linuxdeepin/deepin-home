// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "../base/const.h"
#include "./homeDaemon.h"
#include "./homeDaemonAdaptor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("deepin");
    QCoreApplication::setOrganizationDomain("deepin.org");
    QCoreApplication::setApplicationName(APP_NAME);
    QApplication app(argc, argv);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.registerService(DEEPIN_HOME_DAEMON_SERVICE)) {
        qDebug() << "DBus Error" << dbus.lastError().message();
        return -1;
    }
    QTranslator translator;
    if (translator.load(QLocale::system().name(), ":/resources/deepin-home-daemon/")) {
        app.installTranslator(&translator);
    }

    HomeDaemon daemon;

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &daemon, &HomeDaemon::exit);

    new HomeDaemonAdaptor(&daemon);
    dbus.registerObject(DEEPIN_HOME_DAEMON_PATH, &daemon);

    daemon.start();
    qDebug() << APP_NAME << "start exec";
    return app.exec();
}
