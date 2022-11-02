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
    QString m_server;
    API *m_api;
    DeepinidClientProxy *m_deepinidClient;
    DeepinidDaemonProxy *m_deepinidDaemon;
    PropertiesChangedProxy *m_deepinidDaemonPropertiesChanged;

public:
    explicit Account(QObject *parent, API *api, QString node);
    ~Account();
    void authorized(QString code, QString state);
    void openForum();
    void login();
    void logout();
    bool isLogin();
    UserInfo getUserInfo();
signals:
    void userInfoChanged();
};

#endif // DEEPIN_HOME_ACCOUNT_H