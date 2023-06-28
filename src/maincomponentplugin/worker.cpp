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
    connect(m_daemon, &HomeDaemonProxy::exited, this, &Worker::exited);
    connect(m_daemon, &HomeDaemonProxy::userInfoChanged, this, &Worker::userInfoChanged);
    connect(m_daemon, &HomeDaemonProxy::messageChanged, this, &Worker::messageChanged);
    connect(m_daemon, &HomeDaemonProxy::showMainWindow, this, &Worker::showMainWindow);
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
QString Worker::getToken()
{
    qCDebug(this->logger) << "get message";
    return m_daemon->getToken();
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