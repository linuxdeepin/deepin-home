// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef WORKER_H
#define WORKER_H

#include <DSysInfo>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDesktopServices>
#include <QHttpMultiPart>
#include <QLoggingCategory>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QTemporaryFile>

#include "homeDaemonProxy.h"
#include "promise.h"

class Worker : public QObject
{
    Q_OBJECT
private:
    HomeDaemonProxy *m_daemon;
    QLoggingCategory logger = QLoggingCategory("worker");
    QString previewImageDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                              + "/.deepin-home";

public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();
public slots:
    QString getNode();
    QString getLanguage();
    QString getMachineID();
    void markRead(QString channel, QString topic, QString uuid);
    bool isRead(QString channel, QString topic, QString uuid);
    void exited();
    void login();
    void logout();
    bool isLogin();
    QMap<QString, QVariant> getUserInfo();
    QString getToken();
    QString getMessages(QString channel, QString topic);
    // 在浏览器打开论坛
    void openForum();
    // 使daemon退出
    void quit();
    // 获取开机自启配置
    bool getAutoStart();
    // 设置开启自启配置
    void setAutoStart(bool enable);
    // 获取本地文件信息
    QMap<QString, QVariant> getFileInfo(QString filepath);
    // 上传本地文件
    QString uploadFile(QString uploadURL, QString filepath, QMap<QString, QVariant> formData);
    // 弹出通知
    void notify(QString title, QString message);
    // 获取系统版本号
    QString sysVersion();
    // 使用系统看图工具预览图片
    void previewImage(QByteArray data);
    // 类似js中的await Promise()
    // 可在qml中将异步函数转为同步执行
    QVariant awaitPromise(QJSValue func);
signals:
    void userInfoChanged();
    void messageChanged();
    void showMainWindow(bool isIconClick);
};

#endif // WORKER_H