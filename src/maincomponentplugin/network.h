// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DEEPIN_HOME_NETWORK_H
#define DEEPIN_HOME_NETWORK_H

#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QQmlNetworkAccessManagerFactory>
#include <QStandardPaths>

class NetworkFactory : public QQmlNetworkAccessManagerFactory
{
private:
    QMutex m_cacheMutex;
    int m_cacheNo = 0;

public:
    NetworkFactory();
    ~NetworkFactory();

    virtual QNetworkAccessManager *create(QObject *parent);
};

#endif // DEEPIN_HOME_NETWORK_H