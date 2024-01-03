// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DEEPIN_HOME_DAEMON_H
#define DEEPIN_HOME_DAEMON_H

#include "account.h"
#include "api.h"
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <QAction>
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QLocale>
#include <QMenu>
#include <QProcess>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QTimer>

#include "applicationManager1Application.h"

using namespace SyncAPI;

class HomeDaemon : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.Home.Daemon")
private:
    // 语言环境
    QString m_language;
    // 分发节点
    QString m_node;
    // 消息渠道
    QStringList m_channels;
    // 节点刷新间隔（单位秒）
    int m_nodeRefreshTime;
    // 渠道刷新任务ID，用于中止旧的任务
    QString m_refreshChannelCronID;
    QMap<QString, QString> m_topicChangeID;

    QSettings m_settings;
    QSystemTrayIcon *m_sysTrayIcon = nullptr;
    QMenu *m_menu = nullptr;
    API *m_api = nullptr;
    Account *m_account = nullptr;
    QLoggingCategory logger = QLoggingCategory("daemon");
    ApplicationManager1Application *m_am_app_home = nullptr;

public:
    explicit HomeDaemon(QObject *parent = nullptr);
    ~HomeDaemon();
    // 启动定时器，循环刷新消息
    void start();
private:
    // 初始化托盘
    void initSysTrayIcon();
    // 生成UUID
    QString newUUID();
    // 每个消息的状态存储在配置文件中
    QString messageSettingKey(QString channel, QString topic, QString uuid);
    // 主流程，更新node信息，并启动定时器刷新渠道消息
    void run();
    // 定时刷新节点
    void refreshNode();
    // 定时刷新渠道
    void refreshChannel(QString cronID, QString channel);
    // 处理消息
    void refreshMessage(QString channel, QString topic, QString changeID);
    // 消息发送到控制中心
    void notify(QString title, QString summary, QString url);
    // 在固定时机提醒填写调查问卷
    void execFirstNotify();
    // 记录运行时间，用于判断是否可以提示填写调查问卷
    void runTimeRecord();

    // 记录主题的change id，change id发生变动才会抓取消息列表
    QString getTopicChangeID(QString channel, QString topic);
    void setTopicChangeID(QString channel, QString topic, QString changeID);

public slots:
    // 客户端登录回调
    void OnAuthorized(QString code, QString state);
    // 获取当前机器码，生成的UUID，非硬件指纹，用于区分用户
    QString getMachineID();
    // 获取服务器地址
    QString getServer();
    // 获取当前语言
    QString getLanguage();
    // 获取分发节点地址，顺便刷新channels，缓存时间由服务器配置
    QString getNode();
    // 获取渠道列表
    QStringList getChannels();
    // 标记消息已读
    void markRead(QString channel, QString topic, QString uuid);
    // 获取消息是否已读
    bool isRead(QString channel, QString topic, QString uuid);
    // 登录账户
    void login();
    // 登出账户
    void logout();
    // 检查系统网络账户是否登陆
    void checkLogin();
    // 当前是否登录
    bool isLogin();
    // 当前登录用户信息
    QMap<QString, QVariant> getUserInfo();
    // 当前登录用户token
    QStringList getToken(QString publicKey);
    // 获取消息列表
    QString getMessages(QString channel, QString topic);
    // 打开论坛
    void openForum();
    // 退出程序
    void quit();
    // 获取开机自启配置
    bool getAutoStart();
    // 设置开启自启配置
    void setAutoStart(bool enable);
    // 获取用户邮箱
    QString getEmail();
    // 设置用户邮箱
    void setEmail(QString email);
    // 因为客户端是单例应用，重复启动会自动退出，并调用daemon的这个接口来通知已存在的客户端激活自己的窗口。
    void activeMainWindows();
    // 获取当前应用版本信息
    QString getVersion();
signals:
    // 用户登录状态变动，客户端收到通知后，可以调用daemon获取新的用户状态
    void userInfoChanged();
    // 消息列表变动，客户端收到通知后，可以调用daemon获取新的消息列表
    void messageChanged();
    // 服务程序退出时发出这个信号，通知客户端同步退出
    void exited();
    // 启动客户端后发出这个信号
    // 因为客户端是单例应用，重复启动会自动退出，用这个信号通知客户端激活自己被最小化窗口
    // 如果客户端的窗口已经是激活状态，单击托盘图标会关闭窗口
    void showMainWindow(bool isIconClick);
};

#endif // DEEPIN_HOME_DAEMON_H