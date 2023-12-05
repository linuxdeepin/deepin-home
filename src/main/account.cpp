// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "account.h"

Account::Account(QObject *parent, API *api, QString server)
    : QObject(parent)
{
    m_api = api;
    m_server = server;
    // DeepinID 客户端 DBUS
    m_deepinidClient = new DeepinidClientProxy("com.deepin.deepinid.Client",
                                               "/com/deepin/deepinid/Client",
                                               QDBusConnection::sessionBus(),
                                               this);
    // DeepinID服务端 DBUS
    m_deepinidDaemon = new DeepinidDaemonProxy("com.deepin.deepinid",
                                               "/com/deepin/deepinid",
                                               QDBusConnection::sessionBus(),
                                               this);
    // 监听DBus属性变动
    m_deepinidDaemonPropertiesChanged = new PropertiesChangedProxy("com.deepin.deepinid",
                                                                   "/com/deepin/deepinid",
                                                                   QDBusConnection::sessionBus(),
                                                                   this);
    // 监听DeepinidDaemon的属性变动
    // 因为qdbus代码生成的dbus属性不包含NOTIFY，通过PropertiesChanged信号来实现
    connect(m_deepinidDaemonPropertiesChanged, &PropertiesChangedProxy::PropertiesChanged, [this] {
        // 跟随deepinid退出登陆
        if (!m_deepinidDaemon->isLogin()) {
            this->m_isLogin = false;
            this->m_token = false;
            emit this->userInfoChanged();
            return;
        }
        // 客户端自己的登陆
        this->clientLogin();
    });
    // 启动后检查deepinid是否登陆
    QTimer::singleShot(1000, this, &Account::checkLogin);
}

Account::~Account() {}

// 登录回调，在deepinid客户端登录后，会回调该接口并传递oauth2 code
// state 用于区分登录用途
void Account::authorized(QString code, QString state)
{
    qCDebug(logger) << "account authorized";
    // 论坛登陆
    if (state == "bbs") {
        auto resp = m_api->getForumURL(m_server, code);
        QDesktopServices::openUrl(resp.getUrl());
        return;
    }
    // 客户端登陆
    if (state == "home") {
        m_logging = false;
        auto resp = m_api->getClientToken(m_server, code);
        m_token = resp.getToken();
        m_isLogin = true;
        emit userInfoChanged();
        return;
    }
}
// 从服务器获取论坛地址，并在浏览器打开。
// 如果客户端已经登录，则进行一次登录，获取到的oauth2 code用于自动登录论坛
void Account::openForum()
{
    // 未登录直接获取论坛地址
    if (!isLogin()) {
        auto resp = m_api->getForumURL(m_server, "");
        QDesktopServices::openUrl(resp.getUrl());
        return;
    }
    // 已登录，则先获取code，再用code获取自动登录的论坛地址
    auto opt = m_api->getLoginOption(m_server);
    // 注册登录回调接口
    m_deepinidClient->Register(opt.getClientId(),
                               DEEPIN_HOME_DAEMON_SERVICE,
                               DEEPIN_HOME_DAEMON_PATH,
                               DEEPIN_HOME_DAEMON_INTERFACE);
    m_deepinidClient->Authorize(opt.getClientId(), opt.getScopes(), opt.getRedirectUrl(), "bbs");
}
// 调用系统的网络账户登陆
void Account::login()
{
    m_deepinidDaemon->Login();
}
// 调用系统的网络账户登出
void Account::logout()
{
    m_deepinidDaemon->Logout();
}

// 返回当前客户端是否登陆
bool Account::isLogin()
{
    return m_isLogin;
}

// 检查系统的网络账户是否登录，如果系统账户登陆，客户端自动登陆
void Account::checkLogin()
{
    qCDebug(logger) << "deepinid login status" << m_deepinidDaemon->isLogin();
    if (m_deepinidDaemon->isLogin()) {
        this->clientLogin();
    }
}

// 客户端自己的登陆
void Account::clientLogin()
{
    if (m_logging) {
        qCDebug(logger) << "client logging in";
        return;
    }
    qCDebug(logger) << "client login";
    m_logging = true;
    // 已登录，则先获取code，再用code获取自动登录的论坛地址
    auto opt = m_api->getLoginOption(m_server);
    // 注册登录回调接口
    m_deepinidClient->Register(opt.getClientId(),
                               DEEPIN_HOME_DAEMON_SERVICE,
                               DEEPIN_HOME_DAEMON_PATH,
                               DEEPIN_HOME_DAEMON_INTERFACE);
    m_deepinidClient->Authorize(opt.getClientId(), opt.getScopes(), opt.getRedirectUrl(), "home");
}

// 获取客户端当前登陆的账户信息
UserInfo Account::getUserInfo()
{
    auto logininfo = m_api->getLoginInfo(m_server, m_token);
    qCDebug(logger) << logininfo.getUserId();
    UserInfo info;
    info.uid = logininfo.getUserId();
    info.nickname = logininfo.getNickname();
    info.avatar_url = logininfo.getAvatar();
    return info;
}

// 获取客户端当前用户的Token
QString Account::getToken()
{
    return m_token;
}
