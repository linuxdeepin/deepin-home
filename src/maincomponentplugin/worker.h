// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef WORKER_H
#define WORKER_H

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>
#include <QObject>

#include "homeDaemonProxy.h"
class Worker : public QObject
{
    Q_OBJECT
private:
    ComDeepinHomeDaemonInterface *m_daemon;

public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();
public slots:
    QString getNode();
    QString getLanguage();
    QString getMachineID();
    void markRead(QString channel, QString topic, QString uuid);
    bool isRead(QString channel, QString topic, QString uuid);
    void exit()
    {
        qDebug() << "daemon exit";
        QCoreApplication::quit();
    };
};

#endif // WORKER_H