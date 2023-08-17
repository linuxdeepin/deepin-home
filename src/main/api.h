// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DEEPIN_HOME_API_H
#define DEEPIN_HOME_API_H

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QObject>
#include <QStandardPaths>
#include <DHClientApi.h>

using namespace DeepinHomeAPI;

class API : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *m_http;
    DHClientApi *_m_client = nullptr;

public:
    explicit API(QObject *parent = nullptr);
    ~API();

    DHHandlers_NodeSelectResponse getNode(QString server, QString machineID);
    DHHandlers_LanguageCodeResponse getLanguage(QString server);
    DHHandlers_PublicTopicsResponse getTopics(QString server, QString channel);
    QList<DHHandlers_ClientMessagesResponse> getMessages(
        QString server, QString channel, QString topic, QString language, QString changeID);
    QJsonDocument getMessagesJSON(
        QString server, QString channel, QString topic, QString language, QString changeID);

    DHHandlers_LoginConfigResponse getLoginOption(QString server);
    DHHandlers_BBSURLResponse getForumURL(QString server, QString code);
    DHHandlers_ClientLoginResponse getClientToken(QString server, QString code);
    DHHandlers_ClientUserInfoResponse getLoginInfo(QString server, QString token);
    DHClientApi *getClient(QString server);
    template<typename T, typename Func1, typename Func2>
    T waitSignal(const typename QtPrivate::FunctionPointer<Func1>::Object *sender, Func1 signal, Func2 errSignal);

signals:
    void signalClientError(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
};

#endif // DEEPIN_HOME_API_H