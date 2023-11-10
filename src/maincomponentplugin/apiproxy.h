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

using namespace SyncAPI;

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
    Env m_env;
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
    void allFeedback(int offset, int limit, QJsonObject opt);
    void getFeedback(QString id);
    void getFeedbackReply(QString id);
    void getLikeFeedback(int offset, int limit);
    void getCollectFeedback(int offset, int limit);
    void getUserFeedback(int offset, int limit, QString type);
    void viewFeedback(QString feedback_id);
    void likeFeedback(QString feedback_id);
    void collectFeedback(QString feedback_id);
    void cancelLikeFeedback(QString feedback_id);
    void cancelCollectFeedback(QString feedback_id);
    void uploadFile(const QString &filepath);
    void createFeedback(const QString &type,
                        const QString &title,
                        const QString &content,
                        const QString &email,
                        const QString &sysVersion,
                        const QString &sysInfo,
                        const QStringList &snapshots);
    void getClientHome();
    void getAboutUS();
    void getInternalTest();
    void getSystemVersion();
signals:
    void signalUnknownError();
    void signalAPIError(int code, QString type, QString msg);
    void signalGetNotifyResp(QJsonArray resp);
    void signalGetQuestionnaireResp(QJsonArray resp);
    void signalAllFeedbackResp(QJsonArray resp);
    void signalGetFeedbackResp(QJsonObject resp);
    void signalGetFeedbackReplyResp(QJsonObject resp);
    void signalGetLikeFeedbackResp(QJsonArray resp);
    void signalGetCollectFeedbackResp(QJsonArray resp);
    void signalGetUserFeedbackResp(QJsonArray resp);
    void signalFeedbackChange(QString feedback_id);
    void signalUploadFileResp(QString filepath, QString uploadID);
    void signalCreateFeedbackResp(QString feedback_id);
    void signalGetClientHomeResp(QJsonObject resp);
    void signalGetAboutUSResp(QString resp);
    void signalGetInternalTestResp(QString resp);
    void signalGetSystemVersionResp(QJsonArray resp);
};

#endif // APIPROXY_H
