// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "../base/const.h"
#include "./homeDaemon.h"
#include "./homeDaemonAdaptor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName("deepin");
    QApplication::setOrganizationDomain("deepin.org");
    // daemon也使用client的应用名
    QApplication::setApplicationName(APP_NAME);
    QApplication::setApplicationVersion(APP_VERSION);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QTranslator translator;
    if (translator.load(QLocale::system().name(), ":/resources/deepin-home-daemon/")) {
        app.installTranslator(&translator);
    }

    HomeDaemon daemon;

    // 如果关闭了开机自启，自动退出
    if (QCoreApplication::arguments().contains("--autostart")) {
        if (!daemon.getAutoStart()) {
            qDebug() << "autostart disable";
            return 0;
        }
    }

    QDBusConnection dbus = QDBusConnection::sessionBus();
    new HomeDaemonAdaptor(&daemon);
    dbus.registerObject(DEEPIN_HOME_DAEMON_PATH, &daemon);

    if (!dbus.registerService(DEEPIN_HOME_DAEMON_SERVICE)) {
        qDebug() << "Register DBus Error" << dbus.lastError().message() << "Process may already be running.";
        return -1;
    }

    daemon.start();
    qDebug() << APP_NAME << "start exec";
    return app.exec();
}
