// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DISKCACHESHARE_H
#define DISKCACHESHARE_H

#include <QAbstractNetworkCache>
#include <QNetworkDiskCache>
#include <QLoggingCategory>
#include <QLockFile>

// 加锁的QNetworkDiskCache对象，用于共用缓存目录
class DiskCacheShare : public QAbstractNetworkCache
{
    Q_OBJECT

public:
    explicit DiskCacheShare(QObject *parent = nullptr);
    ~DiskCacheShare();

    QString cacheDirectory() const;
    void setCacheDirectory(const QString &cacheDir);

    qint64 maximumCacheSize() const;
    void setMaximumCacheSize(qint64 size);

    qint64 cacheSize() const override;
    QNetworkCacheMetaData metaData(const QUrl &url) override;
    void updateMetaData(const QNetworkCacheMetaData &metaData) override;
    QIODevice *data(const QUrl &url) override;
    bool remove(const QUrl &url) override;
    QIODevice *prepare(const QNetworkCacheMetaData &metaData) override;
    void insert(QIODevice *device) override;

    QNetworkCacheMetaData fileMetaData(const QString &fileName) const;

public Q_SLOTS:
    void clear() override;

private:
    QNetworkDiskCache *m_cache = nullptr;
    QLockFile *m_lock = nullptr;
    QLoggingCategory logger = QLoggingCategory("cache");
};

#endif // DISKCACHESHARE_H
