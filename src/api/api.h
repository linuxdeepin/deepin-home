// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DEEPIN_HOME_API_H
#define DEEPIN_HOME_API_H

#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QObject>
#include <QSharedPointer>
#include <QStandardPaths>

#include "diskCacheShare.h"
#include <DHClientApi.h>
#include <getFeedbackParam.h>

using namespace DeepinHomeAPI;

namespace SyncAPI {
class APIException : public QException
{
public:
    int err_code = 0;
    QString err_type = "";
    QString err_msg = "";
    void raise() const override { throw *this; }
    APIException *clone() const override { return new APIException(*this); }
};

class API : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *m_http;
    void init();

    template<typename T, typename Func1, typename Func2>
    T waitSignal(const typename QtPrivate::FunctionPointer<Func1>::Object *sender,
                 Func1 signal,
                 Func2 errSignal);

public:
    explicit API(QObject *parent = nullptr);
    explicit API(QString cacheName, QObject *parent = nullptr);
    explicit API(QNetworkDiskCache *cacheDisk, QObject *parent = nullptr);
    ~API();

    DHHandlers_NodeSelectResponse getNode(QString server, QString machineID);
    DHHandlers_LanguageCodeResponse getLanguage(QString server);
    DHHandlers_PublicTopicsResponse getTopics(QString server, QString channel);
    QList<DHHandlers_ClientMessagesResponse> getMessages(QString server,
                                                         QString channel,
                                                         QString topic,
                                                         QString language);
    DHHandlers_LoginConfigResponse getLoginOption(QString server);
    DHHandlers_BBSURLResponse getForumURL(QString server, QString code);
    DHHandlers_ClientLoginResponse getClientToken(QString server, QString code);
    DHHandlers_ClientUserInfoResponse getLoginInfo(QString server, QString token);
    QSharedPointer<DHClientApi> getClient(QString server, QString token = "");
    QList<DHFeedback_PublicListResponse> getFeedback(const QString &server,
                                                     const QString &language,
                                                     const int &offset,
                                                     const int &limit,
                                                     const GetFeedbackOptionalParam &param);
    QList<DHFeedback_PublicListResponse> getFeedback(const QString &server,
                                                     const QString &language,
                                                     const int &offset,
                                                     const int &limit,
                                                     const QStringList publicID);
    QList<DeepinHomeAPI::DHFeedback_PublicStatResponse> getFeedbackStat(const QString &server,
                                                                        const QStringList &publicID);
    QList<DeepinHomeAPI::DHFeedback_UserRelationListResponse> getFeedbackRelation(
        const QString &server, const QString &token, int offset, int limit, const QString &relation);
    QList<DeepinHomeAPI::DHFeedback_UserRelationListResponse> getFeedbackRelation(
        const QString &server,
        const QString &token,
        int offset,
        int limit,
        const QStringList &publicID,
        const QStringList &relation);
    QList<DeepinHomeAPI::DHFeedback_UserListResponse> getUserFeedback(const QString &server,
                                                                      const QString &token,
                                                                      const int &offset,
                                                                      const int &limit,
                                                                      QString type);
    void createUserFeedbackRelation(const QString &server,
                                    const QString &token,
                                    const QString &feedback_id,
                                    const QString &relation);
    void removeUserFeedbackRelation(const QString &server,
                                    const QString &token,
                                    const QString &feedback_id,
                                    const QString &relation);
    void addFeedbackView(const QString &server, const QString &feedback_id);
    QList<DHFeedback_PublicReplyResponse> getFeedbackReply(const QString &server,
                                                           const QString &feedback_id);
    QString uploadFile(const QString &server, const QString &token, const QString &filepath);
    QString createFeedback(const QString &server,
                           const QString &token,
                           const QString &type,
                           const QString &language,
                           const QString &title,
                           const QString &content,
                           const QString &email,
                           const QString &sysVersion,
                           const QString &sysInfo,
                           const QStringList &snapshots);
    QString getSetting(const QString &server, const QString &settingKey);
};

} // namespace SyncAPI
#endif // DEEPIN_HOME_API_H