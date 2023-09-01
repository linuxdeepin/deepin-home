// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later
#ifndef APIPROXY_H
#define APIPROXY_H

#include <QQmlContext>
#include <QQmlParserStatus>
#include <QtConcurrent>

#include "../base/const.h"
#include "api.h"
#include "homeDaemonProxy.h"
#include "worker.h"

struct Env
{
    QString server;
    QString language;
    QString cachename;
    QString token;
    bool isLogin;
};

class APIProxy : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
private:
    Env env;
    Worker *m_worker = nullptr;
    QLoggingCategory logger = QLoggingCategory("apiproxy");

public:
    explicit APIProxy(QObject *parent = nullptr);
    void componentComplete();
    void classBegin(){};

private:
    template<typename T, typename Func2>
    void waitFuture(QFuture<T> future, Func2 receiver);
    void errorHandle(int code, QString type, QString msg);
    void desktopNotify(QString title, QString message);
    const Env getEnv();
public slots:
    void getNotify();
    void getQuestionnaire();
    void getFeedback(int offset, int limit, QString type);
    void getFeedback(QString id);
    void getLikeFeedback(int offset, int limit);
    void getCollectFeedback(int offset, int limit);
    void getUserFeedback(int offset, int limit, QString type);

signals:
    void signalUnknownError();
    void signalAPIError(int code, QString type, QString msg);
    void signalGetNotifyResp(QJsonArray resp);
    void signalGetQuestionnaireResp(QJsonArray resp);
    void signalGetFeedbackResp(QJsonArray resp);
    void signalGetLikeFeedbackResp(QJsonArray resp);
    void signalGetCollectFeedbackResp(QJsonArray resp);
    void signalGetUserFeedbackResp(QJsonArray resp);
};

#endif // APIPROXY_H
