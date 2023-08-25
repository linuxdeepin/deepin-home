// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QProcessEnvironment>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include <QVariant>

#include "apiproxy.h"
#include "maincomponentplugin.h"

DQUICK_USE_NAMESPACE

MainComponentPlugin::MainComponentPlugin(QObject *parent)
    : QObject(parent)
{}

MainComponentPlugin::~MainComponentPlugin() {}

QUrl MainComponentPlugin::mainComponentPath() const
{
    // 返回程序的主控件部分 qml 文件，请确保该文件存在
    return QUrl("qrc:///main.qml");
}

void MainComponentPlugin::initialize(QQmlApplicationEngine *engine)
{
    // 设置上下文，用于在qml环境抛出异常
    QQmlEngine::setContextForObject(&m_worker, engine->rootContext());
    engine->setNetworkAccessManagerFactory(&m_network);
    qmlRegisterType<APIProxy>("APIProxy", 1, 0, "APIProxy");
    engine->rootContext()->setContextProperty("worker", &m_worker);
}
