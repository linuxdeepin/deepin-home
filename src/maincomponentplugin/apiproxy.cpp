// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

#include "apiproxy.h"

template<typename T>
QJsonArray toJsonArray(QList<T> list)
{
    QJsonArray arr;
    for (auto msg : list) {
        arr.append(msg.asJsonObject());
    }
    return arr;
}

APIProxy::APIProxy(QObject *parent)
    : QObject(parent)
{
    m_daemon = new HomeDaemonProxy(DEEPIN_HOME_DAEMON_SERVICE,
                                   DEEPIN_HOME_DAEMON_PATH,
                                   QDBusConnection::sessionBus(),
                                   this);
    m_server = m_daemon->getNode();
    m_language = m_daemon->getLanguage();
}

void APIProxy::getNotify()
{
    QtConcurrent::run([this]() {
        QPointer p(this);
        API api("client_http_cache");
        auto resp = api.getMessages(m_server,
                                    DEEPIN_HOME_CHANNEL_PUBLIC,
                                    DEEPIN_HOME_TOPIC_NEWS,
                                    m_language);
        if (p != nullptr) {
            emit signalGetNotifyResp(toJsonArray(resp));
        }
    });
}

void APIProxy::getQuestionnaire()
{
    QtConcurrent::run([this]() {
        QPointer p(this);
        API api("client_http_cache");
        auto resp = api.getMessages(m_server,
                                    DEEPIN_HOME_CHANNEL_PUBLIC,
                                    DEEPIN_HOME_TOPIC_QUESTIONS,
                                    m_language);
        if (p != nullptr) {
            emit signalGetQuestionnaireResp(toJsonArray(resp));
        }
    });
}
