// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "api.h"

API::API(QObject *parent)
    : QObject(parent)
{
    init();
}

API::API(QString cacheName, QObject *parent)
    : QObject(parent)
{
    init();
    auto diskCache = new QNetworkDiskCache(this);
    auto cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    diskCache->setCacheDirectory(cacheDir + "/" + cacheName);
    m_http->setCache(diskCache);
}

API::API(QNetworkDiskCache *cacheDisk, QObject *parent)
    : QObject(parent)
{
    init();
    m_http->setCache(cacheDisk);
}

API::~API() {}

void API::init()
{
    m_http = new QNetworkAccessManager(this);
    m_http->setRedirectPolicy(QNetworkRequest::SameOriginRedirectPolicy);
}

// 阻塞性等待信号发生，并返回信号发射的值
template<typename T, typename Func1, typename Func2>
T API::waitSignal(const typename QtPrivate::FunctionPointer<Func1>::Object *sender,
                  Func1 signal,
                  Func2 errSignal)
{
    T result;
    QString err;
    QEventLoop loop;
    connect(sender, signal, &loop, [&loop, &result, &err](DHHttpRequestWorker *worker, T resp) {
        if (worker->getHttpResponseCode() >= 400) {
            err = QString("http code %1").arg(worker->getHttpResponseCode());
        }
        auto headers = worker->getResponseHeaders();
        if (!headers["Content-Type"].startsWith("application/json")) {
            err = QString("http content: %1 != application/json").arg(headers["Content-Type"]);
        }
        result = resp;
        loop.quit();
    });
    connect(sender, errSignal, &loop, [&loop, &err](T resp, auto error_type, QString error_str) {
        err = error_str;
        loop.quit();
    });
    loop.exec();
    if (!err.isEmpty()) {
        qWarning() << "http request error:" << err;
        throw err;
    }
    return result;
}

// 获取客户端实例，暂时先共用同一个实例，之后会根据server初始化多个实例
QSharedPointer<DHClientApi> API::getClient(QString server)
{
    auto client = QSharedPointer<DHClientApi>(new DHClientApi());
    client->setNetworkAccessManager(m_http);
    client->setNewServerForAllOperations(server + "/api/v1");
    client->addHeaders("User-Agent", QString("DeepinHomeClient/%1").arg(APP_VERSION));
    return client;
}

// 从服务器获取语言映射
DHHandlers_LanguageCodeResponse API::getLanguage(QString server)
{
    auto client = getClient(server);
    client->getLanguageCode(QLocale::system().name());
    return waitSignal<DHHandlers_LanguageCodeResponse>(client.data(),
                                                       &DHClientApi::getLanguageCodeSignalFull,
                                                       &DHClientApi::getLanguageCodeSignalE);
}
// 获取分发节点和消息渠道列表
DHHandlers_NodeSelectResponse API::getNode(QString server, QString machineID)
{
    auto client = getClient(server);
    client->getNodes(machineID);
    return waitSignal<DHHandlers_NodeSelectResponse>(client.data(),
                                                     &DHClientApi::getNodesSignalFull,
                                                     &DHClientApi::getNodesSignalE);
}
// 获取消息主题列表
DHHandlers_PublicTopicsResponse API::getTopics(QString server, QString channel)
{
    auto client = getClient(server);
    client->getTopics(channel);
    return waitSignal<DHHandlers_PublicTopicsResponse>(client.data(),
                                                       &DHClientApi::getTopicsSignalFull,
                                                       &DHClientApi::getTopicsSignalE);
}

// 获取消息列表
QList<DHHandlers_ClientMessagesResponse> API::getMessages(
    QString server, QString channel, QString topic, QString language, QString changeID)
{
    auto client = getClient(server);
    client->getMessages(channel, topic, language);
    return waitSignal<QList<DHHandlers_ClientMessagesResponse>>(client.data(),
                                                                &DHClientApi::getMessagesSignalFull,
                                                                &DHClientApi::getMessagesSignalE);
}

// 获取登录选项，包含oauth2 id、scope等
DHHandlers_LoginConfigResponse API::getLoginOption(QString server)
{
    auto client = getClient(server);
    client->getLoginConfig();
    return waitSignal<DHHandlers_LoginConfigResponse>(client.data(),
                                                      &DHClientApi::getLoginConfigSignalFull,
                                                      &DHClientApi::getLoginConfigSignalE);
}

// 获取论坛地址
// 如果code不为空，服务器生成论坛自动登录的地址
DHHandlers_BBSURLResponse API::getForumURL(QString server, QString code)
{
    auto client = getClient(server);
    client->getBBSURL(code);
    return waitSignal<DHHandlers_BBSURLResponse>(client.data(),
                                                 &DHClientApi::getBBSURLSignalFull,
                                                 &DHClientApi::getBBSURLSignalE);
}

// 获取用户Token
DHHandlers_ClientLoginResponse API::getClientToken(QString server, QString code)
{
    qDebug() << "get token";
    auto client = getClient(server);
    DHHandlers_ClientLoginRequest req;
    req.setCode(code);
    client->clientLogin(req);
    return waitSignal<DHHandlers_ClientLoginResponse>(client.data(),
                                                      &DHClientApi::clientLoginSignalFull,
                                                      &DHClientApi::clientLoginSignalE);
}

// 获取当前登陆用户的信息
DHHandlers_ClientUserInfoResponse API::getLoginInfo(QString server, QString token)
{
    qDebug() << "login info";
    auto client = getClient(server);
    client->setApiKey("Authorization", "Bearer " + token);
    client->getLoginInfo();
    return waitSignal<DHHandlers_ClientUserInfoResponse>(client.data(),
                                                         &DHClientApi::getLoginInfoSignalFull,
                                                         &DHClientApi::getLoginInfoSignalE);
}