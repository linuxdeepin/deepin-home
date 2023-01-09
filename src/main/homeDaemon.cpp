// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "homeDaemon.h"

HomeDaemon::HomeDaemon(QObject *parent)
    : QObject(parent)
{
    // 网络请求
    m_api = new API(this);

    // 初始化系统托盘
    m_menu = new QMenu();
    initSysTrayIcon();

    // 记录启动次数和运行时间（用于提醒填写调查问卷）
    if (m_settings.value("firstNotify").toBool()) {
        m_settings.remove("bootCount");
        m_settings.remove("runTime");
    } else {
        QTimer::singleShot(60 * 60 * 1000, this, &HomeDaemon::runTimeRecord);
        auto count = m_settings.value("bootCount", 0).toInt() + 1;
        if (QCoreApplication::arguments().contains("--autostart")) {
            m_settings.setValue("bootCount", count);
        }
        if (count >= 4) {
            execFirstNotify();
        }
    }
}

HomeDaemon::~HomeDaemon()
{
    delete m_menu;
}

void HomeDaemon::runTimeRecord()
{
    if (!m_settings.value("firstNotify").toBool()) {
        auto count = m_settings.value("runTime", 0).toInt() + 1;
        m_settings.setValue("runTime", count);
        if (count >= 12) {
            execFirstNotify();
        }
        QTimer::singleShot(60 * 60 * 1000, this, &HomeDaemon::runTimeRecord);
    }
}

void HomeDaemon::initSysTrayIcon()
{
    // 系统托盘
    m_sysTrayIcon = new QSystemTrayIcon(this);
    m_sysTrayIcon->setIcon(QIcon::fromTheme("deepin-home"));
    m_sysTrayIcon->setToolTip(tr("Deepin Home"));
    // 托盘菜单
    auto showMainAction = new QAction(tr("Show main window"), this);
    connect(m_sysTrayIcon, &QSystemTrayIcon::activated, showMainAction, &QAction::triggered);
    connect(showMainAction, &QAction::triggered, this, [this] {
        QProcess::startDetached("deepin-home", QStringList());
        emit showMainWindow();
    });
    auto exitAction = new QAction(tr("Exit"), this);
    connect(exitAction, &QAction::triggered, this, &HomeDaemon::exited);
    connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    m_menu->addAction(showMainAction);
    m_menu->addAction(exitAction);
    m_sysTrayIcon->setContextMenu(m_menu);
    m_sysTrayIcon->show();
}

QString HomeDaemon::getMachineID()
{
    auto machineID = m_settings.value("machineID").toString();
    qDebug() << "try to get machine id from setting";
    if (!machineID.isEmpty()) {
        return machineID;
    }
    machineID = newUUID();
    m_settings.setValue("machineID", machineID);
    qDebug() << "new machine id saved to setting";
    return machineID;
}

// 获取服务器地址
QString HomeDaemon::getServer()
{
    auto server = m_settings.value("server").toString();
    if (!server.isEmpty()) {
        return server;
    }
    server = DEEPIN_HOME_SERVER;
    m_settings.setValue("server", server);
    return server;
}

// 获取当前语言
QString HomeDaemon::getLanguage()
{
    if (m_language.isEmpty()) {
        m_language = m_api->getLanguage(getServer());
    }
    return m_language;
};

// 获取分发节点地址，顺便刷新channels，缓存时间由服务器配置
QString HomeDaemon::getNode()
{
    if (m_node.isEmpty()) {
        refreshNode();
    }
    return m_node;
}

// 获取渠道列表
QStringList HomeDaemon::getChannels()
{
    if (m_node.isEmpty()) {
        refreshNode();
    }
    return m_channels;
}
// 标记消息已读
void HomeDaemon::markRead(QString channel, QString topic, QString uuid)
{
    auto settingKey = messageSettingKey(channel, topic, uuid);
    qDebug() << "mark read" << settingKey;
    m_settings.beginGroup("messages");
    m_settings.setValue(settingKey, "read");
    m_settings.endGroup();
}
// 获取消息是否已读
bool HomeDaemon::isRead(QString channel, QString topic, QString uuid)
{
    auto settingKey = messageSettingKey(channel, topic, uuid);
    m_settings.beginGroup("messages");
    auto value = m_settings.value(settingKey).toString();
    m_settings.endGroup();
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
// 启动定时器，循环刷新消息
void HomeDaemon::start()
{
    // 初始化 account 服务
    if (m_account == nullptr) {
        QTimer::singleShot(1000, this, &HomeDaemon::run);
        m_account = new Account(this, m_api, getServer());
        connect(m_account, &Account::userInfoChanged, this, &HomeDaemon::userInfoChanged);
    }
}
// 主流程，更新node信息，并启动定时器刷新渠道消息
void HomeDaemon::run()
{
    qDebug() << "Refresh Node";
    // 用于停止旧的定时器
    auto cronID = newUUID();
    m_refreshChannelCronID = cronID;

    // 如果执行发生异常，在一个小时后重试
    auto nextRefreshTime = 60 * 60;
    try {
        refreshNode();
        // 如果执行无异常，服从服务器调控
        nextRefreshTime = m_nodeRefreshTime;
        for (auto channel : getChannels()) {
            QTimer::singleShot(1000, this, [cronID, channel, this] {
                refreshChannel(cronID, channel);
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
    auto node = m_api->getNode(getServer(), getMachineID());

    m_node = node.server;
    m_channels = node.channels;
    m_nodeRefreshTime = node.refresh_time;
}
// 定时刷新单个渠道
void HomeDaemon::refreshChannel(QString cronID, QString channel)
{
    qDebug() << "Refresh Channel" << channel;

    // 如果执行发生异常，在十分钟后重试
    auto nextRefreshTime = 60 * 10;
    try {
        auto topics = m_api->getTopics(getNode(), channel);
        auto changed = false;
        for (auto topic : topics.list) {
            auto lastChangeID = getTopicChangeID(channel, topic.name);
            if (topic.change_id != lastChangeID) {
                message(channel, topic.name, topic.change_id);
                setTopicChangeID(channel, topic.name, topic.change_id);
                changed = true;
            }
        }
        if (changed) {
            emit messageChanged();
        }
        nextRefreshTime = topics.refresh_time;
    } catch (...) {
        qWarning() << "Refresh Channel Error";
    }
    qDebug() << "topic next refresh will be in the" << nextRefreshTime << "seconds";
    // 延迟再次运行
    QTimer::singleShot(nextRefreshTime * 1000, this, [this, cronID, channel] {
        // 如果cron id刷新，则任务中断
        if (cronID != m_refreshChannelCronID) {
            return;
        }
        refreshChannel(cronID, channel);
    });
}
// 在固定时机提醒填写调查问卷
void HomeDaemon::execFirstNotify()
{
    qDebug() << "execFirstNotify";
    try {
        auto messages = m_api->getMessages(getNode(),
                                           DEEPIN_HOME_CHANNEL_PUBLIC,
                                           DEEPIN_HOME_TOPIC_QUESTIONS,
                                           getLanguage(),
                                           getTopicChangeID(DEEPIN_HOME_CHANNEL_PUBLIC,
                                                            DEEPIN_HOME_TOPIC_QUESTIONS));
        for (auto message : messages) {
            if (!message.top) {
                continue;
            }
            // 发送消息通知
            notify(message.title, message.summary, message.url);
            m_settings.setValue("firstNotify", true);
            qDebug() << "send first notify" << message.title << message.summary;
        }
    } catch (...) {
        qWarning() << "Network Error";
    }
}
// 处理消息
void HomeDaemon::message(QString channel, QString topic, QString changeID)
{
    qDebug() << "refresh message" << channel << topic;
    auto messages = m_api->getMessages(getNode(), channel, topic, getLanguage(), changeID);
    QStringList ids;
    m_settings.beginGroup("messages");
    for (auto message : messages) {
        auto settingKey = messageSettingKey(channel, topic, message.uuid);
        // 记录所有消息，便于下面进行清理
        ids << settingKey;

        if (!message.notify) {
            continue;
        }
        // 是否已通知
        if (!m_settings.value(settingKey).toString().isEmpty()) {
            continue;
        }
        m_settings.setValue(settingKey, "notify");
        // 发送消息通知
        notify(message.title, message.summary, message.url);
        qDebug() << "send notify" << settingKey << message.title << message.summary;
    }
    // 清理过期的消息
    auto prefix = QString("%1_%2").arg(channel).arg(topic);
    for (auto key : m_settings.childKeys()) {
        if (key.startsWith(prefix) && !ids.contains(key)) {
            qDebug() << "remove" << key;
            m_settings.remove(key);
        }
    }
    m_settings.endGroup();
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

// 记录主题的change id，避免返回刷新消息列表
QString HomeDaemon::getTopicChangeID(QString channel, QString topic)
{
    auto key = QString("%1_%2").arg(channel).arg(topic);
    return m_topicChangeID[key];
}
void HomeDaemon::setTopicChangeID(QString channel, QString topic, QString changeID)
{
    auto key = QString("%1_%2").arg(channel).arg(topic);
    m_topicChangeID[key] = changeID;
}

// 客户端登录后回调
void HomeDaemon::OnAuthorized(QString code, QString state)
{
    m_account->authorized(code, state);
}
// 登录账户
void HomeDaemon::login()
{
    m_account->login();
}
// 登出账户
void HomeDaemon::logout()
{
    m_account->logout();
}
// 当前是否登录
bool HomeDaemon::isLogin()
{
    return m_account->isLogin();
}
// 获取当前登录用户信息
QMap<QString, QVariant> HomeDaemon::getUserInfo()
{
    QMap<QString, QVariant> m;
    auto info = m_account->getUserInfo();
    m["uid"] = info.uid;
    m["nickname"] = info.nickname;
    m["avatar_url"] = info.avatar_url;
    return m;
}
// 获取消息列表数据
QString HomeDaemon::getMessages(QString channel, QString topic)
{
    auto doc = m_api->getMessagesJSON(getNode(),
                                      channel,
                                      topic,
                                      getLanguage(),
                                      getTopicChangeID(channel, topic));
    return QString(doc.toJson());
}
// 打开论坛
void HomeDaemon::openForum()
{
    m_account->openForum();
}
// 退出daemon
void HomeDaemon::quit()
{
    QCoreApplication::quit();
}

// 获取开机自启配置
bool HomeDaemon::getAutoStart()
{
    return m_settings.value("autostart", true).toBool();
}
// 设置开启自启配置
void HomeDaemon::setAutoStart(bool enable)
{
    return m_settings.setValue("autostart", enable);
}