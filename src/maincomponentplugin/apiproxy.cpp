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

    connect(this, &APIProxy::signalAPIError, this, &APIProxy::APIErrorHandle);
}

void APIProxy::getNotify()
{
    auto future = QtConcurrent::run([this]() {
        API api(m_cachename);
        auto resp = api.getMessages(m_server,
                                    DEEPIN_HOME_CHANNEL_PUBLIC,
                                    DEEPIN_HOME_TOPIC_NEWS,
                                    m_language);
        return toJsonArray(resp);
    });
    waitFuture(future, [this](auto resp) { emit this->signalGetNotifyResp(resp); });
}
// 获取调查问卷
void APIProxy::getQuestionnaire()
{
    auto future = QtConcurrent::run([this]() {
        API api(m_cachename);
        auto resp = api.getMessages(m_server,
                                    DEEPIN_HOME_CHANNEL_PUBLIC,
                                    DEEPIN_HOME_TOPIC_QUESTIONS,
                                    m_language);
        return toJsonArray(resp);
    });
    waitFuture(future, [this](auto resp) { emit this->signalGetQuestionnaireResp(resp); });
}

// 弹出桌面通知
void APIProxy::desktopNotify(QString title, QString message)
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

// 等待future完成后匿名函数，统一处理异常
template<typename T, typename Func2>
void APIProxy::waitFuture(QFuture<T> future, Func2 receiver)
{
    QPointer p(this);
    auto watcher = new QFutureWatcher<T>();
    connect(watcher, &QFutureWatcher<T>::finished, [this, watcher, receiver, p]() {
        watcher->deleteLater();
        // 判断this是否已销毁
        if (p == nullptr) {
            return;
        }
        try {
            receiver(watcher->result());
        } catch (APIException exp) {
            qCDebug(logger) << "api exception" << exp.err_code;
            emit this->signalAPIError(exp.err_code, exp.err_type, exp.err_msg);
        } catch (...) {
            qCDebug(logger) << "unknown exception";
            emit this->signalUnknownError();
        }
    });
    watcher->setFuture(future);
}

// 处理全局错误，业务错误由qml自行处理
void APIProxy::APIErrorHandle(int code, QString type, QString msg)
{
    switch (code) {
        // 请求过于频繁
    case 429:
        this->desktopNotify(tr("You have been making too many requests. Please try again later."),
                            "");
        break;
    }
};