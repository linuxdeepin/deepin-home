// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include <DAppLoader>
#include <QDBusConnection>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

DQUICK_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.registerService("com.deepin.Home")) {
        qDebug() << "DBus Error";
        return -1;
    }
#ifdef LOCALLIBPATH
    DAppLoader appLoader(APP_NAME, LOCALLIBPATH);
#else
    DAppLoader appLoader(APP_NAME);
#endif
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    return appLoader.exec(argc, argv);
}
