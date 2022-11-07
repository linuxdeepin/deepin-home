// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "api.h"
#include "../base/const.h"

API::API(QObject *parent)
    : QObject(parent)
{
    auto diskCache = new QNetworkDiskCache(this);
    auto cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    diskCache->setCacheDirectory(cacheDir + "/http_cache");
    m_http = new QNetworkAccessManager(this);
    m_http->setCache(diskCache);
}

API::~API() {}

// 封装http get请求
QJsonDocument API::get(const QUrl &url)
{
    QString err;
    // 如果网络错误，再重试2次
    for (auto i = 0; i < 3; i++) {
        auto req = QNetworkRequest(url);
        req.setHeader(QNetworkRequest::UserAgentHeader,
                      QString("DeepinHomeClient/%1").arg(DEEPIN_HOME_VERSION));
        auto reply = m_http->get(req);
        QEventLoop eventLoop;
        connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();
        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Network Error" << reply->errorString();
            err = reply->errorString();
            continue;
        }
        QByteArray replyData = reply->readAll();
        reply->deleteLater();
        reply = nullptr;
        return QJsonDocument::fromJson(replyData);
    }
    throw err;
}
// 封装http post请求
QJsonDocument API::post(const QUrl &url, QJsonDocument body)
{
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  QString("DeepinHomeClient/%1").arg(DEEPIN_HOME_VERSION));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    auto reply = m_http->post(req, body.toJson());
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

// 从服务器获取语言映射
QString API::getLanguage(QString server)
{
    auto local = QLocale::system();
    auto url = QString("%1/api/v1/public/language/%2").arg(server).arg(local.name());
    auto doc = get(url).object();
    return doc.value("code").toString();
}
// 获取分发节点和消息渠道列表
Node API::getNode(QString server, QString machineID)
{
    Node node;
    auto url = QString("%1/api/v1/public/machine/%2/node").arg(server).arg(machineID);
    auto doc = get(url).object();
    node.server = doc.value("server").toString();
    node.channels = doc.value("channels").toVariant().toStringList();
    node.refresh_time = doc.value("refresh_time").toInt();
    return node;
}
// 获取消息主题列表
Topics API::getTopics(QString server, QString channel)
{
    Topics topics;
    auto url = QString("%1/api/v1/public/channel/%2/topics").arg(server).arg(channel);
    auto doc = get(url).object();
    topics.refresh_time = doc.value("refresh_time").toInt();
    topics.list = QList<Topic>();
    for (auto v : doc.value("topics").toArray()) {
        auto t = v.toObject();
        Topic topic;
        topic.name = t.value("name").toString();
        topic.change_id = t.value("change_id").toString();
        topics.list << topic;
    }
    return topics;
}
// 获取消息列表
QList<Message> API::getMessages(
    QString server, QString channel, QString topic, QString language, QString changeID)
{
    auto url = QString("%1/api/v1/public/channel/%2/topic/%3/messages?language=%4&change_id=%5")
                   .arg(server)
                   .arg(channel)
                   .arg(topic)
                   .arg(language)
                   .arg(changeID);
    auto list = get(url).array();

    QList<Message> result;
    for (auto v : list) {
        auto obj = v.toObject();
        Message msg;
        msg.uuid = obj.value("uuid").toString();
        msg.url = obj.value("url").toString();
        msg.title = obj.value("title").toString();
        msg.summary = obj.value("summary").toString();
        msg.content = obj.value("content").toString();
        msg.start_at = obj.value("start_at").toString();
        msg.end_at = obj.value("end_at").toString();
        msg.notify = obj.value("notify").toBool();
        msg.top = obj.value("top").toBool();
        result << msg;
    }
    return result;
}
// 获取消息列表的原始json数据，便于传递给qml
QJsonDocument API::getMessagesJSON(
    QString server, QString channel, QString topic, QString language, QString changeID)
{
    auto url = QString("%1/api/v1/public/channel/%2/topic/%3/messages?language=%4&change_id=%5")
                   .arg(server)
                   .arg(channel)
                   .arg(topic)
                   .arg(language)
                   .arg(changeID);
    return get(url);
}

// 获取登录选项，包含oauth2 id、scope等
LoginOption API::getLoginOption(QString server)
{
    LoginOption opt;
    auto url = QString("%1/api/v1/public/login/info").arg(server);
    auto obj = get(url).object();
    opt.client_id = obj.value("client_id").toString();
    opt.redirect_url = obj.value("redirect_url").toString();
    opt.scopes = obj.value("scopes").toVariant().toStringList();
    return opt;
}

// 获取论坛地址
// 如果code不为空，服务器会论坛自动登录的地址
QString API::getForumURL(QString server, QString code)
{
    QJsonObject obj;
    obj["code"] = code;
    auto url = QString("%1/api/v1/public/login/bbs_url").arg(server);
    auto doc = post(url, QJsonDocument(obj));
    return doc.object().value("url").toString();
}