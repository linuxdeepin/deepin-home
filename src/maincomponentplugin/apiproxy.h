// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later
#ifndef APIPROXY_H
#define APIPROXY_H

#include <QtConcurrent>

#include "../base/const.h"
#include "api.h"
#include "homeDaemonProxy.h"

class APIProxy : public QObject
{
    Q_OBJECT
private:
    QString m_server;
    QString m_language;
    HomeDaemonProxy *m_daemon;

public:
    explicit APIProxy(QObject *parent = nullptr);

public slots:
    void getNotify();
    void getQuestionnaire();

signals:
    void signalGetNotifyResp(QJsonArray resp);
    void signalGetQuestionnaireResp(QJsonArray resp);
};

#endif // APIPROXY_H