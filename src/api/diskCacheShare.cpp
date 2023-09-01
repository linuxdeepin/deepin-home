// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "diskCacheShare.h"

DiskCacheShare::DiskCacheShare(QObject *parent)
{
    m_cache = new QNetworkDiskCache(this);
};

DiskCacheShare::~DiskCacheShare()
{
    delete m_lock;
    m_lock = nullptr;
}

QString DiskCacheShare::cacheDirectory() const
{
    qCDebug(logger) << "get cacheDirectory";
    return m_cache->cacheDirectory();
}

void DiskCacheShare::setCacheDirectory(const QString &cacheDir)
{
    qCDebug(logger) << "set cacheDirectory" << cacheDir;
    m_cache->setCacheDirectory(cacheDir);
    // 初始化文件锁
    if (m_lock != nullptr) {
        delete m_lock;
    }
    m_lock = new QLockFile(cacheDir + "/" + "cache.lock");
}

qint64 DiskCacheShare::maximumCacheSize() const
{
    qCDebug(logger) << "get maximumCacheSize";
    return m_cache->maximumCacheSize();
}

void DiskCacheShare::setMaximumCacheSize(qint64 size)
{
    qCDebug(logger) << "set maximumCacheSize";
    return m_cache->setMaximumCacheSize(size);
}

qint64 DiskCacheShare::cacheSize() const
{
    qCDebug(logger) << "get cacheSize";
    return m_cache->cacheSize();
}

QNetworkCacheMetaData DiskCacheShare::metaData(const QUrl &url)
{
    qCDebug(logger) << "get metaData" << url.toString();
    m_lock->lock();
    auto data = m_cache->metaData(url);
    m_lock->unlock();
    return data;
}

void DiskCacheShare::updateMetaData(const QNetworkCacheMetaData &metaData)
{
    qCDebug(logger) << "update metaData" << metaData.url().toString();
    m_lock->lock();
    m_cache->updateMetaData(metaData);
    m_lock->unlock();
}

QIODevice *DiskCacheShare::data(const QUrl &url)
{
    qCDebug(logger) << "get data" << url.toString();
    m_lock->lock();
    auto device = m_cache->data(url);
    m_lock->unlock();
    return device;
}

bool DiskCacheShare::remove(const QUrl &url)
{
    qCDebug(logger) << "remove data" << url.toString();
    m_lock->lock();
    auto ok = m_cache->remove(url);
    m_lock->unlock();
    return ok;
}

QIODevice *DiskCacheShare::prepare(const QNetworkCacheMetaData &metaData)
{
    qCDebug(logger) << "prepare" << metaData.url().toString();
    m_lock->lock();
    auto device = m_cache->prepare(metaData);
    m_lock->unlock();
    return device;
}

void DiskCacheShare::insert(QIODevice *device)
{
    qCDebug(logger) << "insert";
    m_lock->lock();
    m_cache->insert(device);
    m_lock->unlock();
}

QNetworkCacheMetaData DiskCacheShare::fileMetaData(const QString &fileName) const
{
    qCDebug(logger) << "get fileMetaData" << fileName;
    m_lock->lock();
    auto data = m_cache->fileMetaData(fileName);
    m_lock->unlock();
    return data;
}

void DiskCacheShare::clear()
{
    qCDebug(logger) << "clear";
    m_lock->lock();
    m_cache->clear();
    m_lock->unlock();
}
