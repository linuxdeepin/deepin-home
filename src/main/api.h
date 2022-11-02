// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DEEPIN_HOME_API_H
#define DEEPIN_HOME_API_H

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QObject>
#include <QStandardPaths>

struct Node
{
    int refresh_time;
    QString server;
    QStringList channels;
};

struct Topic
{
    QString name;
    QString change_id;
};

struct Topics
{
    int refresh_time;
    QList<Topic> list;
};

struct Message
{
    QString uuid;
    QString url;
    QString title;
    QString summary;
    QString content;
    QString start_at;
    QString end_at;
    bool notify;
    bool top;
};
Q_DECLARE_METATYPE(Message)

struct LoginOption
{
    QString client_id;
    QString redirect_url;
    QStringList scopes;
};

class API : QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *m_http;

    QJsonDocument get(const QUrl &url);
    QJsonDocument post(const QUrl &url, QJsonDocument body);

public:
    explicit API(QObject *parent = nullptr);
    ~API();

    Node getNode(QString server, QString machineID);
    QString getLanguage(QString server);
    Topics getTopics(QString server, QString channel);
    QList<Message> getMessages(
        QString server, QString channel, QString topic, QString language, QString changeID);
    QJsonDocument getMessagesJSON(
        QString server, QString channel, QString topic, QString language, QString changeID);

    LoginOption getLoginOption(QString server);
    QString getForumURL(QString server, QString code);
};

#endif // DEEPIN_HOME_API_H