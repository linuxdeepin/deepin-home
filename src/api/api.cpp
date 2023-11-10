// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "api.h"

namespace SyncAPI {
API::API(QObject *parent)
    : QObject(parent)
{
    init();
}

API::API(QString cacheName, QObject *parent)
    : QObject(parent)
{
    init();
    auto diskCache = new DiskCacheShare(this);
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
    APIException exp;
    QEventLoop loop;
    connect(sender, signal, &loop, [&loop, &result, &exp](DHHttpRequestWorker *worker, T resp) {
        auto code = worker->getHttpResponseCode();
        if (code >= 400) {
            exp.err_code = code;
            exp.err_type = "http";
            exp.err_msg = QString("http code %1").arg(worker->getHttpResponseCode());
        }
        auto headers = worker->getResponseHeaders();
        if (!headers["Content-Type"].startsWith("application/json")) {
            exp.err_code = 600;
            exp.err_type = "http";
            exp.err_msg = QString("http content: %1 != application/json")
                              .arg(headers["Content-Type"]);
        }
        result = resp;
        loop.quit();
    });
    connect(sender, errSignal, &loop, [this, &loop, &exp](auto *worker, auto err_type, auto err_str) {
        exp.err_code = worker->getHttpResponseCode();
        exp.err_type = err_type;
        exp.err_msg = err_str;
        if (exp.err_code == 0) {
            exp.err_code = -1;
        }
        loop.quit();
    });
    loop.exec();
    if (exp.err_code != 0) {
        throw exp;
    }
    return result;
}

// 获取客户端实例，暂时先共用同一个实例，之后会根据server初始化多个实例
QSharedPointer<DHClientApi> API::getClient(QString server, QString token)
{
    auto client = QSharedPointer<DHClientApi>(new DHClientApi());
    client->setParent(this);
    client->setNetworkAccessManager(m_http);
    client->setNewServerForAllOperations(server + "/api/v1");
    client->addHeaders("User-Agent", QString("DeepinHomeClient/%1").arg(APP_VERSION));
    if (!token.isEmpty()) {
        client->setApiKey("Authorization", "Bearer " + token);
    }
    return client;
}

// 获取反馈
QList<DHFeedback_PublicListResponse> API::getFeedback(const QString &server,
                                                      const QString &language,
                                                      const int &offset,
                                                      const int &limit,
                                                      const GetFeedbackOptionalParam &param)
{
    auto client = getClient(server);
    client->getFeedback(offset,
                        limit,
                        param.type,
                        param.status,
                        param.public_id,
                        language,
                        param.order,
                        param.version,
                        param.keyword);
    return waitSignal<QList<DHFeedback_PublicListResponse>>(client.data(),
                                                            &DHClientApi::getFeedbackSignalFull,
                                                            &DHClientApi::getFeedbackSignalEFull);
}

// 根据public id获取反馈
QList<DHFeedback_PublicListResponse> API::getFeedback(const QString &server,
                                                      const QString &language,
                                                      const int &offset,
                                                      const int &limit,
                                                      const QStringList publicID)
{
    auto client = getClient(server);
    client->getFeedback(offset,
                        limit,
                        OptionalParam<QString>(),
                        OptionalParam<QList<QString>>(),
                        publicID,
                        language);
    return waitSignal<QList<DHFeedback_PublicListResponse>>(client.data(),
                                                            &DHClientApi::getFeedbackSignalFull,
                                                            &DHClientApi::getFeedbackSignalEFull);
}

// 获取反馈的统计信息
QList<DeepinHomeAPI::DHFeedback_PublicStatResponse> API::getFeedbackStat(const QString &server,
                                                                         const QStringList &publicID)
{
    auto client = getClient(server);
    client->getFeedbackStat(publicID);
    return waitSignal<QList<DeepinHomeAPI::DHFeedback_PublicStatResponse>>(
        client.data(),
        &DHClientApi::getFeedbackStatSignalFull,
        &DHClientApi::getFeedbackStatSignalEFull);
}

QList<DeepinHomeAPI::DHFeedback_UserListResponse> API::getUserFeedback(
    const QString &server, const QString &token, const int &offset, const int &limit, QString type)
{
    auto client = getClient(server, token);
    client->getUserFeedback(offset, limit, type);
    return waitSignal<QList<DeepinHomeAPI::DHFeedback_UserListResponse>>(
        client.data(),
        &DHClientApi::getUserFeedbackSignalFull,
        &DHClientApi::getUserFeedbackSignalEFull);
}

// 创建用户和反馈的关联关系
void API::createUserFeedbackRelation(const QString &server,
                                     const QString &token,
                                     const QString &feedback_id,
                                     const QString &relation)
{
    auto client = getClient(server, token);
    client->createFeedbackRelation(feedback_id, relation);
    waitSignal<DHFeedback_UserRelationResposne>(client.data(),
                                                &DHClientApi::createFeedbackRelationSignalFull,
                                                &DHClientApi::createFeedbackRelationSignalEFull);
}

// 移除用户和反馈的关联关系
void API::removeUserFeedbackRelation(const QString &server,
                                     const QString &token,
                                     const QString &feedback_id,
                                     const QString &relation)
{
    auto client = getClient(server, token);
    client->removeFeedbackRelation(feedback_id, relation);
    waitSignal<DHFeedback_UserRelationResposne>(client.data(),
                                                &DHClientApi::removeFeedbackRelationSignalFull,
                                                &DHClientApi::removeFeedbackRelationSignalEFull);
}

// 增加反馈的查看次数
void API::addFeedbackView(const QString &server, const QString &feedback_id)
{
    auto client = getClient(server);
    client->addFeedbackView(feedback_id);
    waitSignal<DHFeedback_PublicViewResponse>(client.data(),
                                              &DHClientApi::addFeedbackViewSignalFull,
                                              &DHClientApi::addFeedbackViewSignalEFull);
}

// 查看反馈的回复
QList<DHFeedback_PublicReplyResponse> API::getFeedbackReply(const QString &server,
                                                            const QString &feedback_id)
{
    auto client = getClient(server);
    client->getFeedbackReply(feedback_id);
    return waitSignal<QList<DHFeedback_PublicReplyResponse>>(
        client.data(),
        &DHClientApi::getFeedbackReplySignalFull,
        &DHClientApi::getFeedbackReplySignalEFull);
}

// 上传文件
QString API::uploadFile(const QString &server, const QString &token, const QString &filepath)
{
    auto info = QFileInfo(filepath);
    auto client = getClient(server, token);
    DHHandlers_PreUploadRequest req;
    req.setName(info.fileName());
    req.setSize(info.size());
    client->preUpload(req);
    auto resp = waitSignal<DHHandlers_PreUploadResponse>(client.data(),
                                                         &DHClientApi::preUploadSignalFull,
                                                         &DHClientApi::preUploadSignalEFull);
    auto formData = resp.getFormData();
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    for (const QString field : resp.getFormData().keys()) {
        auto value = formData[field];
        QHttpPart textPart;
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QString("form-data; name=\"%1\"").arg(field));
        textPart.setBody(formData[field].toUtf8());
        multiPart->append(textPart);
    }
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QString("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QString("form-data; name=\"file\"; filename=\"%1\"").arg(info.fileName()));
    QFile *file = new QFile(filepath, multiPart);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    multiPart->append(imagePart);

    QUrl url(resp.getUrl());
    QNetworkRequest request(url);
    QNetworkReply *reply = m_http->post(request, multiPart);
    multiPart->setParent(reply);
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        throw reply->errorString();
    }
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() >= 400) {
        throw reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    }
    return resp.getId();
}

// 创建用户反馈
QString API::createFeedback(const QString &server,
                            const QString &token,
                            const QString &type,
                            const QString &language,
                            const QString &title,
                            const QString &content,
                            const QString &email,
                            const QString &sysVersion,
                            const QString &sysInfo,
                            const QStringList &snapshots)
{
    auto client = getClient(server, token);
    DHFeedback_CreateFeedbackRequest req;
    req.setType(type);
    req.setLanguage(language);
    req.setTitle(title);
    req.setContent(content);
    req.setEmail(email);
    req.setVersion(sysVersion);
    req.setSystemInfo(sysInfo);
    req.setScreenshots(snapshots);
    client->createFeedback(req);
    auto resp
        = waitSignal<DHFeedback_CreateFeedbackResponse>(client.data(),
                                                        &DHClientApi::createFeedbackSignalFull,
                                                        &DHClientApi::createFeedbackSignalEFull);
    return resp.getPublicId();
}

// 获取通用配置
QString API::getSetting(const QString &server, const QString &settingKey)
{
    auto client = getClient(server);
    client->getSetting(settingKey);
    auto resp = waitSignal<DHHandlers_GetStetingResponse>(client.data(),
                                                          &DHClientApi::getSettingSignalFull,
                                                          &DHClientApi::getSettingSignalEFull);
    return resp.getValue();
};

// 获取和用户有关联的反馈
QList<DeepinHomeAPI::DHFeedback_UserRelationListResponse> API::getFeedbackRelation(
    const QString &server, const QString &token, int offset, int limit, const QString &relation)
{
    auto client = getClient(server, token);
    client->getFeedbackRelation(offset,
                                limit,
                                OptionalParam<QList<QString>>(),
                                QStringList(relation));
    return waitSignal<QList<DeepinHomeAPI::DHFeedback_UserRelationListResponse>>(
        client.data(),
        &DHClientApi::getFeedbackRelationSignalFull,
        &DHClientApi::getFeedbackRelationSignalEFull);
}

// 获取用户和反馈的关联关系
QList<DeepinHomeAPI::DHFeedback_UserRelationListResponse> API::getFeedbackRelation(
    const QString &server,
    const QString &token,
    int offset,
    int limit,
    const QStringList &publicID,
    const QStringList &relation)
{
    auto client = getClient(server, token);
    client->getFeedbackRelation(offset, limit, publicID, relation);
    return waitSignal<QList<DeepinHomeAPI::DHFeedback_UserRelationListResponse>>(
        client.data(),
        &DHClientApi::getFeedbackRelationSignalFull,
        &DHClientApi::getFeedbackRelationSignalEFull);
}

// 从服务器获取语言映射
DHHandlers_LanguageCodeResponse API::getLanguage(QString server)
{
    auto client = getClient(server);
    client->getLanguageCode(QLocale::system().name());
    return waitSignal<DHHandlers_LanguageCodeResponse>(client.data(),
                                                       &DHClientApi::getLanguageCodeSignalFull,
                                                       &DHClientApi::getLanguageCodeSignalEFull);
}

// 获取分发节点和消息渠道列表
DHHandlers_NodeSelectResponse API::getNode(QString server, QString machineID)
{
    auto client = getClient(server);
    client->getNodes(machineID);
    return waitSignal<DHHandlers_NodeSelectResponse>(client.data(),
                                                     &DHClientApi::getNodesSignalFull,
                                                     &DHClientApi::getNodesSignalEFull);
}
// 获取消息主题列表
DHHandlers_PublicTopicsResponse API::getTopics(QString server, QString channel)
{
    auto client = getClient(server);
    client->getTopics(channel);
    return waitSignal<DHHandlers_PublicTopicsResponse>(client.data(),
                                                       &DHClientApi::getTopicsSignalFull,
                                                       &DHClientApi::getTopicsSignalEFull);
}

// 获取消息列表
QList<DHHandlers_ClientMessagesResponse> API::getMessages(QString server,
                                                          QString channel,
                                                          QString topic,
                                                          QString language)
{
    auto client = getClient(server);
    client->getMessages(channel, topic, language);
    return waitSignal<QList<DHHandlers_ClientMessagesResponse>>(client.data(),
                                                                &DHClientApi::getMessagesSignalFull,
                                                                &DHClientApi::getMessagesSignalEFull);
}

// 获取登录选项，包含oauth2 id、scope等
DHHandlers_LoginConfigResponse API::getLoginOption(QString server)
{
    auto client = getClient(server);
    client->getLoginConfig();
    return waitSignal<DHHandlers_LoginConfigResponse>(client.data(),
                                                      &DHClientApi::getLoginConfigSignalFull,
                                                      &DHClientApi::getLoginConfigSignalEFull);
}

// 获取论坛地址
// 如果code不为空，服务器生成论坛自动登录的地址
DHHandlers_BBSURLResponse API::getForumURL(QString server, QString code)
{
    auto client = getClient(server);
    DHHandlers_BBSURLRequest req;
    req.setCode(code);
    client->getBBSURL(req);
    return waitSignal<DHHandlers_BBSURLResponse>(client.data(),
                                                 &DHClientApi::getBBSURLSignalFull,
                                                 &DHClientApi::getBBSURLSignalEFull);
}

// 获取用户Token
DHHandlers_ClientLoginResponse API::getClientToken(QString server, QString code)
{
    auto client = getClient(server);
    DHHandlers_ClientLoginRequest req;
    req.setCode(code);
    client->clientLogin(req);
    return waitSignal<DHHandlers_ClientLoginResponse>(client.data(),
                                                      &DHClientApi::clientLoginSignalFull,
                                                      &DHClientApi::clientLoginSignalEFull);
}

// 获取当前登陆用户的信息
DHHandlers_ClientUserInfoResponse API::getLoginInfo(QString server, QString token)
{
    auto client = getClient(server, token);
    client->getLoginInfo();
    return waitSignal<DHHandlers_ClientUserInfoResponse>(client.data(),
                                                         &DHClientApi::getLoginInfoSignalFull,
                                                         &DHClientApi::getLoginInfoSignalEFull);
}

} // namespace SyncAPI