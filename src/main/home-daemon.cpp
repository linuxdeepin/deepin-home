// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "./home-daemon.h"

HomeDaemon::HomeDaemon(QObject *parent)
    : QObject(parent)
{
    // 网络请求缓存目录
    auto diskCache = new QNetworkDiskCache(this);
    auto cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    diskCache->setCacheDirectory(cacheDir + "/http_cache");
    mHttp = new QNetworkAccessManager(this);
    mHttp->setCache(diskCache);

    mMenu = new QMenu();
    initSysTrayIcon();
}

HomeDaemon::~HomeDaemon()
{
    delete mMenu;
}

void HomeDaemon::initSysTrayIcon()
{
    // 系统托盘
    mSysTrayIcon = new QSystemTrayIcon(this);
    mSysTrayIcon->setIcon(QIcon::fromTheme("deepin-home"));
    mSysTrayIcon->setToolTip("深度之家");
    // 托盘菜单
    auto showMainAction = new QAction("显示主界面", this);
    connect(mSysTrayIcon, &QSystemTrayIcon::activated, this, [] {
        QProcess mainProcess;
        mainProcess.startDetached("deepin-home", QStringList());
    });
    connect(showMainAction, &QAction::triggered, this, [] {
        QProcess mainProcess;
        mainProcess.startDetached("deepin-home", QStringList());
    });
    auto exitAction = new QAction("退出", this);
    connect(exitAction, &QAction::triggered, this, [this] {
        emit exit();
        QCoreApplication::quit();
    });
    mMenu->addAction(showMainAction);
    mMenu->addAction(exitAction);
    mSysTrayIcon->setContextMenu(mMenu);
    mSysTrayIcon->show();
}

QString HomeDaemon::getMachineID()
{
    auto machineID = settings.value("machineID").toString();
    qDebug() << "try to get machine id from setting";
    if (!machineID.isEmpty()) {
        return machineID;
    }
    machineID = newUUID();
    settings.setValue("machineID", machineID);
    qDebug() << "new machine id saved to setting";
    return machineID;
}

// 获取服务器地址
QString HomeDaemon::getServer()
{
    auto server = settings.value("server").toString();
    if (!server.isEmpty()) {
        return server;
    }
    server = "https://home.deepin.org";
    settings.setValue("server", server);
    return server;
}

// 获取当前语言
QString HomeDaemon::getLanguage()
{
    if (language.isEmpty()) {
        auto local = QLocale::system();
        auto url = QString("%1/api/v1/public/language/%2").arg(getServer()).arg(local.name());
        auto doc = fetch(url).object();
        language = doc.value("code").toString();
    }
    return language;
};

// 获取分发节点地址，顺便刷新channels，缓存时间由服务器配置
QString HomeDaemon::getNode()
{
    if (node.isEmpty()) {
        refreshNode();
    }
    return node;
}

// 获取渠道列表
QStringList HomeDaemon::getChannels()
{
    if (node.isEmpty()) {
        refreshNode();
    }
    return channels;
}
// 标记消息已读
void HomeDaemon::markRead(QString channel, QString topic, QString uuid)
{
    auto settingKey = messageSettingKey(channel, topic, uuid);
    qDebug() << "mark read" << settingKey;
    settings.beginGroup("messages");
    settings.setValue(settingKey, "read");
    settings.endGroup();
}
// 获取消息是否已读
bool HomeDaemon::isRead(QString channel, QString topic, QString uuid)
{
    auto settingKey = messageSettingKey(channel, topic, uuid);
    settings.beginGroup("messages");
    auto value = settings.value(settingKey).toString();
    settings.endGroup();
    qDebug() << "get message status" << settingKey << value;
    return value == "read";
}

QString HomeDaemon::newUUID()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}
// 每个消息的状态存储在配置文件中
QString HomeDaemon::messageSettingKey(QString channel, QString topic, QString uuid)
{
    return QString("%1_%2_%3").arg(channel).arg(topic).arg(uuid);
}
// 封装http get请求
QJsonDocument HomeDaemon::fetch(const QUrl &url)
{
    auto reply = mHttp->get(QNetworkRequest(url));
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Network Error" << reply->errorString();
        throw reply->errorString();
    }
    QByteArray replyData = reply->readAll();
    reply->deleteLater();
    reply = nullptr;
    return QJsonDocument::fromJson(replyData);
}
// 启动定时器，循环刷新消息
void HomeDaemon::start()
{
    QTimer::singleShot(1000, this, &HomeDaemon::run);
}
// 主流程，更新node信息，并启动定时器刷新渠道消息
void HomeDaemon::run()
{
    qDebug() << "Refresh Node";
    // 用于停止旧的定时器
    auto cronID = newUUID();
    refreshChannelCronID = cronID;

    // 如果执行发生异常，在一个小时后重试
    auto nextRefreshTime = 60 * 60;
    try {
        refreshNode();
        auto currentNode = getNode();
        // 如果执行无异常，服从服务器调控
        nextRefreshTime = nodeRefreshTime;
        for (auto channel : getChannels()) {
            QTimer::singleShot(1000, this, [cronID, currentNode, channel, this] {
                refreshChannel(cronID, currentNode, channel);
            });
        }
    } catch (...) {
        qWarning() << "Refresh Node Error";
    }
    qDebug() << "node next refresh will be in the" << nextRefreshTime << "seconds";
    QTimer::singleShot(nextRefreshTime * 1000, this, &HomeDaemon::run);
}
void HomeDaemon::refreshNode()
{
    if (QDateTime::currentDateTime() < nodeLastRefresh.addSecs(nodeRefreshTime)) {
        return;
    }
    auto url = QString("%1/api/v1/public/machine/%2/node").arg(getServer()).arg(getMachineID());
    auto doc = fetch(url).object();
    node = doc.value("server").toString();
    channels = doc.value("channels").toVariant().toStringList();
    auto refresh_time = doc.value("refresh_time").toInt();
    // 默认一分钟刷新一次，服务器可根据负载进行调控
    if (refresh_time == 0) {
        refresh_time = 60;
    }
    nodeRefreshTime = refresh_time;
    nodeLastRefresh = QDateTime::currentDateTime();
    qDebug() << "refresh node" << node;
}
// 定时刷新单个渠道
void HomeDaemon::refreshChannel(QString cronID, QString node, QString channel)
{
    qDebug() << "Refresh Channel" << channel;

    // 如果执行发生异常，在十分钟后重试
    auto nextRefreshTime = 60 * 10;
    try {
        auto url = QString("%1/api/v1/public/channel/%2/topics").arg(node).arg(channel);
        auto doc = fetch(url).object();

        for (auto v : doc.value("topics").toArray()) {
            auto t = v.toObject();
            auto topic = t.value("name").toString();
            auto settingKey = QString("topics/%1_%2_changeID").arg(channel).arg(topic);
            auto currentChangeID = t.value("change_id").toString();
            auto lastChangeID = settings.value(settingKey);
            if (currentChangeID != lastChangeID) {
                settings.setValue(settingKey, currentChangeID);
                message(node, channel, topic, currentChangeID);
            }
        }

        auto refresh_time = doc.value("refresh_time").toInt();
        if (refresh_time == 0) {
            // 默认一分钟刷新一次，服务器可根据负载和渠道重要程度进行调控
            refresh_time = 60;
        }
        nextRefreshTime = refresh_time;
    } catch (...) {
        qWarning() << "Refresh Channel Error";
    }
    qDebug() << "topic next refresh will be in the" << nextRefreshTime << "seconds";
    // 延迟1分钟再次运行
    QTimer::singleShot(nextRefreshTime * 1000, this, [=] {
        // 如果任务中断，则不再运行
        if (cronID != refreshChannelCronID) {
            return;
        }
        refreshChannel(cronID, node, channel);
    });
}
// 处理消息
void HomeDaemon::message(QString node, QString channel, QString topic, QString changeID)
{
    qDebug() << "refresh message" << channel << topic;

    auto url = QString("%1/api/v1/public/channel/%2/topic/%3/messages?language=%4&change_id=%5")
                   .arg(node)
                   .arg(channel)
                   .arg(topic)
                   .arg(getLanguage())
                   .arg(changeID);
    auto list = fetch(url).array();
    QStringList ids;
    settings.beginGroup("messages");
    for (auto v : list) {
        auto message = v.toObject();
        auto uuid = message.value("uuid").toString();
        if (!message.value("notify").toBool()) {
            continue;
        }
        auto settingKey = messageSettingKey(channel, topic, uuid);
        // 记录所有消息，便于下面进行清理
        ids << settingKey;
        // 是否已通知
        if (!settings.value(settingKey).toString().isEmpty()) {
            continue;
        }
        settings.setValue(settingKey, "notify");
        // 发送消息通知
        auto title = message.value("title").toString();
        auto summary = message.value("summary").toString();
        auto url = message.value("url").toString();
        notify(title, summary, url);
        qDebug() << "send notify" << settingKey << title << summary;
    }
    // 清理过期的消息
    auto prefix = QString("%1_%2").arg(channel).arg(topic);
    for (auto key : settings.childKeys()) {
        if (key.startsWith(prefix) && !ids.contains(key)) {
            qDebug() << "remove" << key;
            settings.remove(key);
        }
    }
    settings.endGroup();
}

// 消息发送到控制中心
void HomeDaemon::notify(QString title, QString summary, QString url)
{
    QStringList actions;
    QVariantMap hints;
    if (!url.isEmpty()) {
        actions << "_open" << tr("View");
        hints["x-deepin-action-_open"] = "xdg-open," + url;
    }
    QDBusInterface notification("org.freedesktop.Notifications",
                                "/org/freedesktop/Notifications",
                                "org.freedesktop.Notifications",
                                QDBusConnection::sessionBus());
    QList<QVariant> args;
    args << QCoreApplication::applicationName() // appname
         << (unsigned int) 0                    // replaces id
         << QCoreApplication::applicationName() // icon
         << title                               // summary (notification title)
         << summary                             // body
         << actions                             // actions
         << hints                               // hints
         << (int) 3000;                         // timeout

    auto res = notification.callWithArgumentList(QDBus::AutoDetect, "Notify", args);
    auto err = res.errorMessage();
    if (!err.isEmpty()) {
        qWarning() << "DBus Error" << err;
    }
}
