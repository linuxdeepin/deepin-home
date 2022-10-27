// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "./worker.h"

Worker::Worker(QObject *parent)
    : QObject(parent)
{
    m_daemon = new ComDeepinHomeDaemonInterface("com.deepin.Home.Daemon",
                                                "/com/deepin/Home/Daemon",
                                                QDBusConnection::sessionBus(),
                                                this);
    connect(m_daemon, SIGNAL(exit()), this, SLOT(exit()));
}

Worker::~Worker() {}

QString Worker::getNode()
{
    qDebug() << "get node";
    return m_daemon->getNode();
};
QString Worker::getLanguage()
{
    qDebug() << "get language";
    return m_daemon->getLanguage();
};
QString Worker::getMachineID()
{
    qDebug() << "get machine id";
    return m_daemon->getMachineID();
};
void Worker::markRead(QString channel, QString topic, QString uuid)
{
    qDebug() << "mark read";
    QList<QVariant> args;
    args << channel << topic << uuid;
    m_daemon->callWithArgumentList(QDBus::NoBlock, "markRead", args);
};
bool Worker::isRead(QString channel, QString topic, QString uuid)
{
    qDebug() << "is read";
    QList<QVariant> args;
    args << channel << topic << uuid;
    QDBusReply<bool> reply = m_daemon->callWithArgumentList(QDBus::BlockWithGui, "isRead", args);
    return reply;
};