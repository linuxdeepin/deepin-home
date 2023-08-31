// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later
#ifndef APIPROXY_H
#define APIPROXY_H

#include <QQmlApplicationEngine>
#include <QtConcurrent>

#include "../base/const.h"
#include "api.h"
#include "homeDaemonProxy.h"

class APIProxy : public QObject
{
    Q_OBJECT
private:
    QString m_server = "";
    QString m_language = "";
    QString m_cachename = "http_cache";
    HomeDaemonProxy *m_daemon = nullptr;
    QLoggingCategory logger = QLoggingCategory("apiproxy");

public:
    explicit APIProxy(QObject *parent = nullptr);
    void desktopNotify(QString title, QString message);
    template<typename T, typename Func2>
    void waitFuture(QFuture<T> future, Func2 receiver);
    void APIErrorHandle(int code, QString type, QString msg);

public slots:
    void getNotify();
    void getQuestionnaire();

signals:
    void signalGetNotifyResp(QJsonArray resp);
    void signalGetQuestionnaireResp(QJsonArray resp);
    void signalAPIError(int code, QString type, QString msg);
    void signalUnknownError();
};

#endif // APIPROXY_H