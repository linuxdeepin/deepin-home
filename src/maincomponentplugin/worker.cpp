// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "./worker.h"
#include "../base/const.h"

Worker::Worker(QObject *parent)
    : QObject(parent)
{
    m_daemon = new HomeDaemonProxy(DEEPIN_HOME_DAEMON_SERVICE,
                                   DEEPIN_HOME_DAEMON_PATH,
                                   QDBusConnection::sessionBus(),
                                   this);
    // 绑定信号
    connect(m_daemon, &HomeDaemonProxy::exited, this, &Worker::exited);
    connect(m_daemon, &HomeDaemonProxy::userInfoChanged, this, &Worker::userInfoChanged);
    connect(m_daemon, &HomeDaemonProxy::messageChanged, this, &Worker::messageChanged);
    connect(m_daemon, &HomeDaemonProxy::showMainWindow, this, &Worker::showMainWindow);
    // 为避免应用升级后接口不兼容，如果客户端和daemon版本不一致，重启一次daemon
    auto clientVersion = QString(APP_VERSION);
    if (!clientVersion.isEmpty()) {
        auto daemonVersion = m_daemon->getVersion();
        if (clientVersion != daemonVersion) {
            m_daemon->quit();
        }
    }
    // 在启动时清理截图预览的缓存目录
    QDir dir(this->previewImageDir);
    dir.removeRecursively();
    qCInfo(this->logger) << "clean up saved screenshots";
    dir.mkpath(this->previewImageDir);
}

Worker::~Worker() {}

QString Worker::getNode()
{
    qCDebug(this->logger) << "get node";
    return m_daemon->getNode();
};

QString Worker::getLanguage()
{
    qCDebug(this->logger) << "get language";
    return m_daemon->getLanguage();
};

QString Worker::getMachineID()
{
    qCDebug(this->logger) << "get machine id";
    return m_daemon->getMachineID();
};

void Worker::markRead(QString channel, QString topic, QString uuid)
{
    qCDebug(this->logger) << "mark read";
    QList<QVariant> args;
    args << channel << topic << uuid;
    m_daemon->callWithArgumentList(QDBus::NoBlock, "markRead", args);
};

bool Worker::isRead(QString channel, QString topic, QString uuid)
{
    qCDebug(this->logger) << "is read";
    QList<QVariant> args;
    args << channel << topic << uuid;
    QDBusReply<bool> reply = m_daemon->callWithArgumentList(QDBus::BlockWithGui, "isRead", args);
    return reply;
};

void Worker::exited()
{
    qCDebug(this->logger) << "daemon exit";
    QCoreApplication::quit();
};

void Worker::login()
{
    qCDebug(this->logger) << "login";
    m_daemon->login();
};

void Worker::logout()
{
    qCDebug(this->logger) << "logout";
    m_daemon->logout();
};

bool Worker::isLogin()
{
    qCDebug(this->logger) << "is login";
    return m_daemon->isLogin();
};

QMap<QString, QVariant> Worker::getUserInfo()
{
    qCDebug(this->logger) << "get user info";
    return m_daemon->getUserInfo();
};
// 获取客户端登陆用户的token
// 为保证token的传输安全，daemon使用client生成的公钥加密token
QString Worker::getToken()
{
    qCDebug(this->logger) << "get token";

    // 生成RSA密钥，v20的 libssl 不支持 EVP_RSA_gen 函数，所以改用 EVP_PKEY_keygen
    auto gctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY_keygen_init(gctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(gctx, 2048);
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_keygen(gctx, &pkey);
    EVP_PKEY_CTX_free(gctx);
    // 写入公钥到内存缓存
    auto mem = BIO_new(BIO_s_mem());
    auto ret = PEM_write_bio_PUBKEY(mem, pkey);
    // 读取公钥到字符串
    char *data;
    ret = BIO_get_mem_data(mem, &data);
    auto publicKey = QString::fromLatin1(data, ret);
    BIO_free_all(mem);
    // 发送公钥给daemon，获取公钥加密后的token
    auto result = m_daemon->getToken(publicKey);
    // 加密可能会分段进行，解密所有分段后拼接到一个字符串
    auto dctx = EVP_PKEY_CTX_new(pkey, NULL);
    EVP_PKEY_decrypt_init(dctx);
    QString token;
    for (auto cipherdata : result.value()) {
        // 加密后的数据进行了base64编码，需要先解码
        auto cipherStr = QByteArray::fromBase64(cipherdata.toLatin1()).toStdString();
        // 使用密钥解密数据
        char decData[256] = {0};
        size_t declen = 256;
        auto rett = EVP_PKEY_decrypt(dctx,
                                     (unsigned char *) decData,
                                     &declen,
                                     (unsigned char *) cipherStr.c_str(),
                                     cipherStr.length());
        // 拼接token
        token += QString::fromLatin1(decData, declen);
    }
    // 释放资源
    EVP_PKEY_CTX_free(dctx);
    EVP_PKEY_free(pkey);
    return token;
}

QString Worker::getMessages(QString channel, QString topic)
{
    qCDebug(this->logger) << "get message";
    return m_daemon->getMessages(channel, topic);
}
void Worker::openForum()
{
    qCDebug(this->logger) << "login bbs";
    m_daemon->openForum();
}
void Worker::quit()
{
    qCDebug(this->logger) << "quit";
    m_daemon->quit();
}

// 获取开机自启配置
bool Worker::getAutoStart()
{
    return m_daemon->getAutoStart();
}
// 设置开启自启配置
void Worker::setAutoStart(bool enable)
{
    m_daemon->setAutoStart(enable);
}
// 获取本地文件信息
QMap<QString, QVariant> Worker::getFileInfo(QString filepath)
{
    auto info = QFileInfo(filepath);
    QMap<QString, QVariant> result;
    result["size"] = info.size();
    result["filename"] = info.fileName();
    result["filepath"] = info.filePath();
    return result;
}

QString Worker::sysVersion()
{
    return Dtk::Core::DSysInfo::minorVersion();
}

void Worker::notify(QString title, QString message)
{
    QStringList actions;
    QVariantMap hints;
    QDBusInterface notification("org.freedesktop.Notifications",
                                "/org/freedesktop/Notifications",
                                "org.freedesktop.Notifications",
                                QDBusConnection::sessionBus());
    QList<QVariant> args;
    args << QCoreApplication::applicationName() // appname
         << (unsigned int) 0                    // replaces id
         << QCoreApplication::applicationName() // icon
         << title                               // summary (notification title)
         << message                             // body
         << actions                             // actions
         << hints                               // hints
         << (int) 5000;                         // timeout

    auto res = notification.callWithArgumentList(QDBus::AutoDetect, "Notify", args);
    auto err = res.errorMessage();

    if (!err.isEmpty()) {
        qWarning() << "DBus Error" << err;
    }
}

// 使用表单上传文件
QString Worker::uploadFile(QString uploadURL, QString filepath, QMap<QString, QVariant> formData)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    foreach (const QString field, formData.keys()) {
        auto value = formData[field].toString();
        QHttpPart textPart;
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QString("form-data; name=\"%1\"").arg(field));
        textPart.setBody(formData[field].toString().toUtf8());
        multiPart->append(textPart);
    }
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"file\"; filename=\"test\""));
    QFile *file = new QFile(filepath, multiPart);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    multiPart->append(imagePart);

    QUrl url(uploadURL);
    QNetworkRequest request(url);
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(request, multiPart);
    reply->deleteLater();
    multiPart->setParent(reply);
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        return reply->errorString();
    }
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() >= 400) {
        return reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    }
    return "";
}
// 使用系统默认的看图工具预览图片
void Worker::previewImage(QByteArray data)
{
    QTemporaryFile tempFile(this->previewImageDir + "/preview");
    tempFile.setAutoRemove(false);
    if (!tempFile.open()) {
        qWarning() << "can not create temp file in " << this->previewImageDir;
        return;
    }
    tempFile.write(data);
    tempFile.close();
    QString filePath = tempFile.fileName();
    qCInfo(this->logger) << "save screenshots to" << filePath << "filesize" << data.length();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
};
// 将异步回调转为同步调用，避免回调嵌套
QVariant Worker::awaitPromise(QJSValue func)
{
    // 初始化promise对象
    auto promise = new MyPromise(this);
    // 设置上下文，用于在qml中抛出异常
    QQmlEngine::setContextForObject(promise, qmlContext(this));
    // 调用func将promise传递到qml
    auto ret = func.call(QJSValueList{qmlEngine(this)->newQObject(promise)});
    // 如果func发生错误，在qml中抛出异常
    if (ret.isError()) {
        qmlEngine(this)->throwError(QJSValue::GenericError, ret.toString());
        return QVariant();
    }
    // 等待promise结束，需要在qml中调用promise对象的resolve/reject,promise才会结束
    // 如果在qml中调用reject，promise会在qml上下文中抛出异常，await反馈空结果
    // 如果在qml中调用resolve，await返回resolve传递的result
    return promise->await();
};