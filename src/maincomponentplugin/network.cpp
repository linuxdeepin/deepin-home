// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "./network.h"
#include <QThread>

NetworkFactory::NetworkFactory() {}

NetworkFactory::~NetworkFactory() {}

QNetworkAccessManager *NetworkFactory::create(QObject *parent)
{
    auto cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    cacheDir += "/http_cache";
    qDebug() << "network factory cache dir:" << cacheDir;

    auto cache = new DiskCacheShare(parent);
    cache->setCacheDirectory(cacheDir);

    QNetworkAccessManager *http = new QNetworkAccessManager(parent);
    http->setCache(cache);
    return http;
}