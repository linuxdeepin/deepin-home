// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef HELLO_H
#define HELLO_H

#include <QAction>
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocale>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QUuid>

class HomeDaemon : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "com.deepin.Home.Daemon")
private:
    // 语言环境
    QString language;
    // 分发节点
    QString node;
    // 消息渠道
    QStringList channels;
    int nodeRefreshTime;
    QDateTime nodeLastRefresh;
    QString refreshChannelCronID;
    QHash<QString, QDateTime> topicRefresh;

    QSettings settings;
    QNetworkAccessManager *mHttp;
    QSystemTrayIcon *mSysTrayIcon;
    QMenu *mMenu;

public:
    explicit HomeDaemon(QObject *parent = nullptr);
    ~HomeDaemon();
    void initSysTrayIcon();
    QString newUUID();
    // 每个消息的状态存储在配置文件中
    QString messageSettingKey(QString channel, QString topic, QString uuid);
    // 封装http get请求
    QJsonDocument fetch(const QUrl &url);
    // 启动定时器，循环刷新消息
    void start();
    // 主流程，更新node信息，并启动定时器刷新渠道消息
    void run();
    void refreshNode();
    // 定时刷新单个渠道
    void refreshChannel(QString cronID, QString node, QString channel);
    // 处理消息
    void message(QString node, QString channel, QString topic, QString changeID);
    // 消息发送到控制中心
    void notify(QString title, QString summary, QString url);
    // 在固定时机提醒填写调查问卷
    void execFirstNotify();
    void runTimeRecord();
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

signals: // 信号
    void exit();
};

#endif // HELLO_H