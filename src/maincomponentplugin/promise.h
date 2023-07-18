// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef MYPROMISE_H
#define MYPROMISE_H

#include <QEventLoop>
#include <QObject>
#include <QQmlEngine>

class MyPromise : public QObject
{
    Q_OBJECT
private:
    bool m_finish = false;
    QEventLoop m_loop;
    QVariant m_result;
    QString m_error;

public:
    explicit MyPromise(QObject *parent = nullptr);
    Q_INVOKABLE void resolve(QVariant result);
    Q_INVOKABLE void reject(QString err);
    Q_INVOKABLE QVariant await();
};

#endif // MYPROMISE_H