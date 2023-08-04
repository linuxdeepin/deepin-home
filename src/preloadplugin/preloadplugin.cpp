// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "preloadplugin.h"
#include "../base/const.h"
#include <DGuiApplicationHelper>
#include <QGuiApplication>
#include <QTranslator>
#include <QUrl>

DQUICK_USE_NAMESPACE

PreloadPlugin::PreloadPlugin(QObject *parent)
    : QObject(parent)
{}

PreloadPlugin::~PreloadPlugin() {}

QUrl PreloadPlugin::preloadComponentPath() const
{
    // 预览组件的 qml 路径
    return QUrl("qrc:///Preload.qml");
}

QGuiApplication *PreloadPlugin::creatApplication(int &argc, char **argv)
{
    QGuiApplication *app = new QGuiApplication(argc, argv);
    app->setOrganizationName("deepin");
    app->setOrganizationDomain("deepin.org");
    app->setApplicationVersion(APP_VERSION);
    // 高分屏
    app->setAttribute(Qt::AA_UseHighDpiPixmaps);
    // 加载翻译
    auto translator = new QTranslator(app);
    if (translator->load(QLocale::system().name(), ":/resources/deepin-home/")) {
        app->installTranslator(translator);
    }
    // 固定为亮色主题
    Dtk::Gui::DGuiApplicationHelper::instance()->setPaletteType(
        Dtk::Gui::DGuiApplicationHelper::ColorType::LightType);
    return app;
}
