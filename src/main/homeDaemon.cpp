// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "homeDaemon.h"
#include "applicationManager1Application.h"

HomeDaemon::HomeDaemon(QObject *parent)
    : QObject(parent)
{
    // 网络请求
    m_api = new API("http_cache", this);
    m_am_app_home = new ApplicationManager1Application("org.desktopspec.ApplicationManager1",
                                   "/org/desktopspec/ApplicationManager1/deepin_2dhome",
                                   QDBusConnection::sessionBus(),
                                   this);
    qCDebug(logger) << m_am_app_home->service() << "isValid" << m_am_app_home->isValid();
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
    // 显示主窗口
    auto showMainAction = new QAction(tr("Show main window"), this);
    connect(m_sysTrayIcon,
            &QSystemTrayIcon::activated,
            this,
            [&](QSystemTrayIcon::ActivationReason reason) {
                if (reason == QSystemTrayIcon::Trigger) {
                    QProcess::startDetached("deepin-home", QStringList());
                    emit showMainWindow(true);
                }
            });
    connect(showMainAction, &QAction::triggered, this, [this] {
        QProcess::startDetached("deepin-home", QStringList());
        emit showMainWindow(false);
    });
    // 论坛
    auto bbsAction = new QAction(tr("Communication"), this);
    connect(bbsAction, &QAction::triggered, this, &HomeDaemon::openForum);
    // 退出
    auto exitAction = new QAction(tr("Exit"), this);
    connect(exitAction, &QAction::triggered, this, &HomeDaemon::exited);
    connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    m_menu->addAction(showMainAction);
    m_menu->addAction(bbsAction);
    m_menu->addAction(exitAction);
    m_sysTrayIcon->setContextMenu(m_menu);
    m_sysTrayIcon->show();
}

QString HomeDaemon::getMachineID()
{
    auto machineID = m_settings.value("machineID").toString();
    qCDebug(logger) << "try to get machine id from setting";
    if (!machineID.isEmpty()) {
        return machineID;
    }
    machineID = newUUID();
    m_settings.setValue("machineID", machineID);
    qCDebug(logger) << "new machine id saved to setting";
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
        auto resp = m_api->getLanguage(getServer());
        m_language = resp.getCode();
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
    qCDebug(logger) << "mark read" << settingKey;
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
    qCDebug(logger) << "get message status" << settingKey << value;
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
    qCDebug(logger) << "Refresh Node";
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
    } catch (APIException exp) {
        qCWarning(logger) << "Refresh Node Error" << exp.err_code << exp.err_msg;
    } catch (...) {
        qCWarning(logger) << "Refresh Node Error";
    }
    qCDebug(logger) << "node next refresh will be in the" << nextRefreshTime << "seconds";
    QTimer::singleShot(nextRefreshTime * 1000, this, &HomeDaemon::run);
}
void HomeDaemon::refreshNode()
{
    auto node = m_api->getNode(getServer(), getMachineID());

    m_node = node.getServer();
    m_channels = node.getChannels();
    m_nodeRefreshTime = node.getRefreshTime();
}
// 定时刷新单个渠道
void HomeDaemon::refreshChannel(QString cronID, QString channel)
{
    qCDebug(logger) << "Refresh Channel" << channel;

    // 如果执行发生异常，在十分钟后重试
    auto nextRefreshTime = 60 * 10;
    try {
        auto topics = m_api->getTopics(getNode(), channel);
        auto changed = false;
        for (auto topic : topics.getTopics()) {
            auto lastChangeID = getTopicChangeID(channel, topic.getName());
            if (topic.getChangeId() != lastChangeID) {
                refreshMessage(channel, topic.getName(), topic.getChangeId());
                setTopicChangeID(channel, topic.getName(), topic.getChangeId());
                changed = true;
            }
        }
        if (changed) {
            emit messageChanged();
        }
        nextRefreshTime = topics.getRefreshTime();
    } catch (APIException exp) {
        qCWarning(logger) << "Refresh Channel Error" << exp.err_code << exp.err_msg;
    } catch (...) {
        qCWarning(logger) << "Refresh Channel Error";
    }
    qCDebug(logger) << "topic next refresh will be in the" << nextRefreshTime << "seconds";
    // 延迟再次运行
    QTimer::singleShot(nextRefreshTime * 1000, this, [this, cronID, channel] {
        // 如果cron id刷新，则任务中断
        if (cronID != m_refreshChannelCronID) {
            return;
        }
        refreshChannel(cronID, channel);
    });
}
// 在恰当的时机提醒填写装机调查问卷
void HomeDaemon::execFirstNotify()
{
    qCDebug(logger) << "execFirstNotify";
    try {
        auto messages = m_api->getMessages(getNode(),
                                           DEEPIN_HOME_CHANNEL_PUBLIC,
                                           DEEPIN_HOME_TOPIC_QUESTIONS,
                                           getLanguage());
        for (auto message : messages) {
            if (message.isTop()) {
                // 发送消息通知
                notify(message.getTitle(), message.getSummary(), message.getUrl());
                m_settings.setValue("firstNotify", true);
                qCDebug(logger) << "send first notify" << message.getTitle() << message.getUrl();
                break;
            }
        }
    } catch (APIException exp) {
        qCWarning(logger) << "Get Notify Error" << exp.err_code << exp.err_msg;
    } catch (...) {
        qCWarning(logger) << "Network Error";
    }
}
// 处理消息
void HomeDaemon::refreshMessage(QString channel, QString topic, QString changeID)
{
    qCDebug(logger) << "refresh message" << channel << topic;
    auto messages = m_api->getMessages(getNode(), channel, topic, getLanguage());
    QStringList ids;
    m_settings.beginGroup("messages");
    for (auto message : messages) {
        auto settingKey = messageSettingKey(channel, topic, message.getUuid());
        // 记录所有消息，便于下面进行清理
        ids << settingKey;

        if (!message.isNotify()) {
            continue;
        }
        // 是否已通知
        if (!m_settings.value(settingKey).toString().isEmpty()) {
            continue;
        }
        m_settings.setValue(settingKey, "notify");
        // 发送消息通知
        notify(message.getTitle(), message.getSummary(), message.getUrl());
        qCDebug(logger) << "send notify" << settingKey << message.getTitle()
                        << message.getSummary();
    }
    // 清理过期的消息
    auto prefix = QString("%1_%2").arg(channel).arg(topic);
    for (auto key : m_settings.childKeys()) {
        if (key.startsWith(prefix) && !ids.contains(key)) {
            qCDebug(logger) << "remove" << key;
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
// 检查系统网络账户是否登陆
void HomeDaemon::checkLogin()
{
    m_account->checkLogin();
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

// 获取用户token
QStringList HomeDaemon::getToken(QString publicKey)
{
    qCDebug(logger) << "get token";
#ifndef QT_DEBUG
    auto pid = connection().interface()->servicePid(message().service());
    qCDebug(logger) << "sender pid" << pid;
    auto sender = QFile::symLinkTarget(QString("/proc/%1/exe").arg(pid));
    if (sender != QString("%1%2").arg(APP_BIN_INSTALL_DIR).arg(APP_NAME)) {
        qCDebug(logger) << "Not allow";
        // 加一个界面通知，避免代码贡献者不知道该接口检查调用者路径
        notify("illegal dbus call", sender + " is not allowed to call the getToken interface", "");
        return {};
    };
#endif
    auto token = m_account->getToken();
    if (token.isEmpty()) {
        qCDebug(logger) << "No token";
        return {};
    }
    // 解析公钥
    auto publicStr = publicKey.toStdString();
    auto mem = BIO_new_mem_buf(publicStr.c_str(), publicStr.length());
    auto pkey = PEM_read_bio_PUBKEY(mem, NULL, NULL, NULL);
    BIO_free_all(mem);
    // 使用公钥加密token，由于rsa无法加密过长的数据，对token进行分段加密
    auto ectx = EVP_PKEY_CTX_new(pkey, NULL);
    EVP_PKEY_encrypt_init(ectx);
    QStringList result;
    // token按100字节分段
    for (int i = 0; i < token.length(); i += 100) {
        auto plaintext = token.mid(i, 100).toStdString();
        // 使用公钥加密分段的数据
        char encData[256] = {0};
        size_t enclen = 256;
        EVP_PKEY_encrypt(ectx,
                         (unsigned char *) encData,
                         &enclen,
                         (unsigned char *) plaintext.c_str(),
                         plaintext.length());
        // 加密后的数据使用base64编码，便于传输
        auto cipherdata = QString(QByteArray(encData, enclen).toBase64());
        result.append(cipherdata);
    }
    // 释放资源
    EVP_PKEY_CTX_free(ectx);
    EVP_PKEY_free(pkey);
    return result;
}

// 获取消息列表数据
QString HomeDaemon::getMessages(QString channel, QString topic)
{
    auto messages = m_api->getMessages(getNode(), channel, topic, getLanguage());
    QJsonArray arr;
    for (auto msg : messages) {
        arr.append(msg.asJsonObject());
    }
    QJsonDocument doc;
    doc.setArray(arr);
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
    qCInfo(logger) << "quit by dbus call";
    QCoreApplication::quit();
}
// 获取开机自启配置
bool HomeDaemon::getAutoStart()
{
    // 新am支持管理全局autostart配置
    if (m_am_app_home->isValid()) {
        return m_am_app_home->autoStart();
    }
    return m_settings.value("autostart", true).toBool();
}
// 设置开启自启配置
void HomeDaemon::setAutoStart(bool enable)
{
    if (m_am_app_home->isValid()) {
        m_am_app_home->setAutoStart(enable);
        return;
    }
    return m_settings.setValue("autostart", enable);
}

// 获取用户邮箱记录
QString HomeDaemon::getEmail()
{
    return m_settings.value("email", "").toString();
}
// 保持用户邮箱记录
void HomeDaemon::setEmail(QString email)
{
    return m_settings.setValue("email", email);
}

// 发出showMainWindow信号，通知客户端激活窗口
void HomeDaemon::activeMainWindows()
{
    emit showMainWindow(false);
}
// 返回应用版本号
QString HomeDaemon::getVersion()
{
    return APP_VERSION;
}