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
    HomeDaemonProxy *m_daemon;

public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();
public slots:
    QString getNode();
    QString getLanguage();
    QString getMachineID();
    void markRead(QString channel, QString topic, QString uuid);
    bool isRead(QString channel, QString topic, QString uuid);
    void exited();
    void login();
    void logout();
    bool isLogin();
    QMap<QString, QVariant> getUserInfo();
    QString getMessages(QString channel, QString topic);
    void openForum();
    void quit();
    // 获取开机自启配置
    bool getAutoStart();
    // 设置开启自启配置
    void setAutoStart(bool enable);
signals:
    void userInfoChanged();
    void messageChanged();
    void showMainWindow();
};

#endif // WORKER_H