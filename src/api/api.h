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

using namespace DeepinHomeAPI;

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
    QSharedPointer<DHClientApi> getClient(QString server);
    template<typename T, typename Func1, typename Func2>
    T waitSignal(const typename QtPrivate::FunctionPointer<Func1>::Object *sender,
                 Func1 signal,
                 Func2 errSignal);
};

#endif // DEEPIN_HOME_API_H