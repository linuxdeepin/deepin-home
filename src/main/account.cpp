// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "account.h"

Account::Account(QObject *parent, API *api, QString server)
    : QObject(parent)
{
    m_api = api;
    m_server = server;

    m_deepinidClient = new DeepinidClientProxy("com.deepin.deepinid.Client",
                                               "/com/deepin/deepinid/Client",
                                               QDBusConnection::sessionBus(),
                                               this);
    m_deepinidDaemon = new DeepinidDaemonProxy("com.deepin.deepinid",
                                               "/com/deepin/deepinid",
                                               QDBusConnection::sessionBus(),
                                               this);
    m_deepinidDaemonPropertiesChanged = new PropertiesChangedProxy("com.deepin.deepinid",
                                                                   "/com/deepin/deepinid",
                                                                   QDBusConnection::sessionBus(),
                                                                   this);
    // 监听DeepinidDaemon的属性变动
    // 因为qdbus代码生成的dbus属性不包含NOTIFY，通过PropertiesChanged信号来实现
    connect(m_deepinidDaemonPropertiesChanged,
            &PropertiesChangedProxy::PropertiesChanged,
            this,
            &Account::userInfoChanged);
}
Account::~Account() {}

// 登录回调，在deepinid客户端登录后，会回调该接口并传递oauth2 code
void Account::authorized(QString code, QString state)
{
    qDebug() << "account authorized";
    // 用于区分登录用途
    if (state == "bbs") {
        auto url = m_api->getForumURL(m_server, code);
        QDesktopServices::openUrl(url);
        return;
    }
}
// 从服务器获取论坛地址，并在浏览器打开。
// 如果客户端已经登录，则进行一次登录，获取到的oauth2 code用于自动登录论坛
void Account::openForum()
{
    // 未登录直接获取论坛地址
    if (!isLogin()) {
        auto url = m_api->getForumURL(m_server, "");
        QDesktopServices::openUrl(url);
        return;
    }
    // 已登录，则先获取code，再用code获取自动登录的论坛地址
    auto opt = m_api->getLoginOption(m_server);
    // 注册登录回调接口
    m_deepinidClient->Register(opt.client_id,
                               DEEPIN_HOME_DAEMON_SERVICE,
                               DEEPIN_HOME_DAEMON_PATH,
                               DEEPIN_HOME_DAEMON_INTERFACE);
    m_deepinidClient->Authorize(opt.client_id, opt.scopes, opt.redirect_url, "bbs");
}
// 登录系统的网络账户
void Account::login()
{
    m_deepinidDaemon->Login();
}
// 登出系统的网络账户
void Account::logout()
{
    m_deepinidDaemon->Logout();
}
// 判断系统当前网络账户是否登录
bool Account::isLogin()
{
    return m_deepinidDaemon->isLogin();
}
// 获取系统当前网络账户的信息
UserInfo Account::getUserInfo()
{
    UserInfo info;
    auto m = m_deepinidDaemon->userInfo();
    info.uid = m["UserID"].toString();
    info.nickname = m["Nickname"].toString();
    info.avatar_url = m["ProfileImage"].toString();
    return info;
}