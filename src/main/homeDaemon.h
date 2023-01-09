// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DEEPIN_HOME_DAEMON_H
#define DEEPIN_HOME_DAEMON_H

#include "../base/const.h"
#include "account.h"
#include "api.h"
#include <QAction>
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>
#include <QHash>
#include <QLocale>
#include <QMenu>
#include <QProcess>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QUuid>

class HomeDaemon : public QObject
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

public:
    explicit HomeDaemon(QObject *parent = nullptr);
    ~HomeDaemon();
    void initSysTrayIcon();
    QString newUUID();
    // 每个消息的状态存储在配置文件中
    QString messageSettingKey(QString channel, QString topic, QString uuid);
    // 启动定时器，循环刷新消息
    void start();
    // 主流程，更新node信息，并启动定时器刷新渠道消息
    void run();
    void refreshNode();
    // 定时刷新单个渠道
    void refreshChannel(QString cronID, QString channel);
    // 处理消息
    void message(QString channel, QString topic, QString changeID);
    // 消息发送到控制中心
    void notify(QString title, QString summary, QString url);
    // 在固定时机提醒填写调查问卷
    void execFirstNotify();
    void runTimeRecord();

    // 记录主题的change id，避免返回刷新消息列表
    QString getTopicChangeID(QString channel, QString topic);
    void setTopicChangeID(QString channel, QString topic, QString changeID);

public slots:
    // 获取当前机器码（用户级别）
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
    // 客户端登录回调
    void OnAuthorized(QString code, QString state);
    // 登录账户
    void login();
    // 登出账户
    void logout();
    // 当前是否登录
    bool isLogin();
    // 当前登录用户信息
    QMap<QString, QVariant> getUserInfo();
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
signals:
    // 用户登录状态变动
    void userInfoChanged();
    // 消息列表变动
    void messageChanged();
    // 程序退出时发出信号
    void exited();
    // 显示主窗口时发出信号
    void showMainWindow();
};

#endif // DEEPIN_HOME_DAEMON_H