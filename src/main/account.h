// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DEEPIN_HOME_ACCOUNT_H
#define DEEPIN_HOME_ACCOUNT_H

#include "../base/const.h"
#include "api.h"
#include "deepinidClientProxy.h"
#include "deepinidDaemonProxy.h"
#include "propertiesChangedProxy.h"
#include <QDesktopServices>
#include <QObject>

using namespace SyncAPI;

struct UserInfo
{
    QString uid;
    QString nickname;
    QString avatar_url;
};

class Account : public QObject
{
    Q_OBJECT
private:
    bool m_isLogin = false;
    bool m_logging = false;
    QString m_server;
    QString m_token;
    API *m_api;
    DeepinidClientProxy *m_deepinidClient;
    DeepinidDaemonProxy *m_deepinidDaemon;
    PropertiesChangedProxy *m_deepinidDaemonPropertiesChanged;
    QLoggingCategory logger = QLoggingCategory("account");
public:
    explicit Account(QObject *parent, API *api, QString node);
    ~Account();
    void authorized(QString code, QString state);
    void openForum();
    void login();
    void logout();
    bool isLogin();
    void clientLogin();
    void checkLogin();
    UserInfo getUserInfo();
    QString getToken();
signals:
    void userInfoChanged();
};

#endif // DEEPIN_HOME_ACCOUNT_H