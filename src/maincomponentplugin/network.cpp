// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "./network.h"
#include <QThread>

NetworkFactory::NetworkFactory() {}

NetworkFactory::~NetworkFactory() {}

QNetworkAccessManager *NetworkFactory::create(QObject *parent)
{
    qDebug() << "network factory"
             << QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

    auto cache = new QNetworkDiskCache(parent);
    auto cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

    m_cacheMutex.lock();
    cacheDir += QString("/qml_http_cache_%1").arg(m_cacheNo++);
    m_cacheMutex.unlock();
    cache->setCacheDirectory(cacheDir);

    QNetworkAccessManager *http = new QNetworkAccessManager(parent);
    http->setCache(cache);
    return http;
}