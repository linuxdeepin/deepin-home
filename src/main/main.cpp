// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include <DAppLoader>
#include <DGuiApplicationHelper>
#include <QDBusConnection>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

DQUICK_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.registerService("com.deepin.Home")) {
        qDebug() << "DBus Error: register com.deepin.Home";
        return -1;
    }
    Dtk::Gui::DGuiApplicationHelper::instance()->setPaletteType(
        Dtk::Gui::DGuiApplicationHelper::ColorType::LightType);
#ifdef LOCALLIBPATH
    DAppLoader appLoader(APP_NAME, LOCALLIBPATH);
#else
    DAppLoader appLoader(APP_NAME);
#endif
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    return appLoader.exec(argc, argv);
}
