// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "./worker.h"

Worker::Worker(QObject *parent)
    : QObject(parent)
{
    daemon = new QDBusInterface("com.deepin.Home.Daemon",
                                "/com/deepin/Home/Daemon",
                                "com.deepin.Home.Daemon",
                                QDBusConnection::sessionBus(),
                                this);
    if (daemon->isValid()) {
        connect(daemon, SIGNAL(exit()), this, SLOT(exit()));
    }
}

Worker::~Worker() {}

QString Worker::getNode()
{
    qDebug() << "get node";
    QDBusReply<QString> reply = daemon->call("getNode");
    return reply;
};
QString Worker::getLanguage()
{
    qDebug() << "get language";
    QDBusReply<QString> reply = daemon->call("getLanguage");
    return reply;
};
QString Worker::getMachineID()
{
    qDebug() << "get machine id";
    QDBusReply<QString> reply = daemon->call("getMachineID");
    return reply;
};
void Worker::markRead(QString channel, QString topic, QString uuid)
{
    qDebug() << "mark read";
    QList<QVariant> args;
    args << channel << topic << uuid;
    daemon->callWithArgumentList(QDBus::NoBlock, "markRead", args);
};
bool Worker::isRead(QString channel, QString topic, QString uuid)
{
    qDebug() << "is read";
    QList<QVariant> args;
    args << channel << topic << uuid;
    QDBusReply<bool> reply = daemon->callWithArgumentList(QDBus::BlockWithGui, "isRead", args);
    return reply;
};