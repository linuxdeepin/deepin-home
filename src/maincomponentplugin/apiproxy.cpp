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
    connect(this, &APIProxy::signalAPIError, this, &APIProxy::errorHandle);
}

// qml组件初始化函数
void APIProxy::componentComplete()
{
    qCDebug(logger) << "apiproxy component complete";
    auto context = qmlContext(this);
    m_worker = context->contextProperty("worker").value<Worker *>();
    env.cachename = "http_cache";
    env.server = m_worker->getNode();
    env.language = m_worker->getLanguage();
    env.isLogin = m_worker->isLogin();
    if (env.isLogin) {
        env.token = m_worker->getToken();
    }
    // 用户变动时刷新token
    connect(m_worker, &Worker::userInfoChanged, this, [this] {
        env.isLogin = m_worker->isLogin();
        if (env.isLogin) {
            env.token = m_worker->getToken();
        }
    });
}

// 弹出桌面通知
void APIProxy::desktopNotify(QString title, QString message = "")
{
    m_worker->notify(title, message);
}

// 获取当前环境
const Env APIProxy::getEnv()
{
    return env;
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
void APIProxy::errorHandle(int code, QString type, QString msg)
{
    // 请求过于频繁
    if (code == 429) {
        auto title = tr("You have been making too many requests, Please try again later.");
        this->desktopNotify(title);
        return;
    }
    if (code == 401) {
        auto title = tr("You need to login to proceed with the subsequent operations.");
        this->desktopNotify(title);
        return;
    }
    // 服务器错误
    if (code >= 500 && code < 600) {
        auto title = tr("Network error, please try later.");
        this->desktopNotify(title);
        return;
    }
};

// 获取新闻通知
void APIProxy::getNotify()
{
    auto env = getEnv();
    auto future = QtConcurrent::run([env]() {
        API api(env.cachename);
        auto resp = api.getMessages(env.server,
                                    DEEPIN_HOME_CHANNEL_PUBLIC,
                                    DEEPIN_HOME_TOPIC_NEWS,
                                    env.language);
        return toJsonArray(resp);
    });
    waitFuture(future, [this](auto resp) { emit this->signalGetNotifyResp(resp); });
}

// 获取调查问卷
void APIProxy::getQuestionnaire()
{
    auto env = getEnv();
    auto future = QtConcurrent::run([env]() {
        API api(env.cachename);
        auto resp = api.getMessages(env.server,
                                    DEEPIN_HOME_CHANNEL_PUBLIC,
                                    DEEPIN_HOME_TOPIC_QUESTIONS,
                                    env.language);
        return toJsonArray(resp);
    });
    waitFuture(future, [this](auto resp) { emit this->signalGetQuestionnaireResp(resp); });
}

QJsonArray fillFeedback(API &api,
                        const Env &env,
                        const QList<DeepinHomeAPI::DHHandlers_FeedbackPublicListResponse> &feedbacks

)
{
    // 获取反馈统计信息
    QList<QString> publicID;
    for (auto feedback : feedbacks) {
        publicID.append(feedback.getPublicId());
    }
    QHash<QString, DHHandlers_PublicStatResponse> statMap;
    for (auto stat : api.getFeedbackStat(env.server, publicID)) {
        statMap.insert(stat.getPublicId(), stat);
    }

    // 获取反馈关联关系
    QHash<QString, DHHandlers_FeedbackUserRelationListResponse> relationMap;
    if (env.isLogin) {
        for (auto relation : api.getFeedbackRelation(env.server,
                                                     env.token,
                                                     0,
                                                     publicID.length() * 2,
                                                     publicID,
                                                     QStringList() << "like"
                                                                   << "collect")) {
            relationMap.insert(relation.getFeedbackId() + relation.getRelation(), relation);
        }
    }

    // 添加统计信息到反馈对象中
    QJsonArray arr;
    for (auto feedback : feedbacks) {
        auto obj = feedback.asJsonObject();
        auto stat = statMap[feedback.getPublicId()];
        obj["view_count"] = stat.is_view_count_Set() ? stat.getViewCount() : 0;
        obj["like_count"] = stat.is_like_count_Set() ? stat.getLikeCount() : 0;
        obj["collect_count"] = stat.is_collect_count_Set() ? stat.getCollectCount() : 0;

        obj["like"] = relationMap.contains(feedback.getPublicId() + "like");
        obj["collect"] = relationMap.contains(feedback.getPublicId() + "collect");

        QJsonArray screenshots;
        for (auto screenshot : feedback.getScreenshots()) {
            screenshots.append(env.server + "/api/v1/public/upload/" + screenshot);
        }
        obj["screenshots"] = screenshots;
        arr.append(obj);
    }
    return arr;
}

// 获取需求广场的列表
void APIProxy::getFeedback(int offset, int limit, QString type)
{
    auto env = getEnv();
    auto future = QtConcurrent::run([env, offset, limit, type]() {
        API api(env.cachename);
        // 获取反馈列表
        auto feedbacks = api.getFeedback(env.server, env.language, offset, limit, type);
        return fillFeedback(api, env, feedbacks);
    });
    waitFuture(future, [this](auto resp) { emit this->signalGetFeedbackResp(resp); });
}

// 获取收藏的反馈列表
void APIProxy::getCollectFeedback(int offset, int limit)
{
    auto env = getEnv();
    auto future = QtConcurrent::run([env, offset, limit]() {
        API api(env.cachename);
        QStringList ids;
        for (auto relation :
             api.getFeedbackRelation(env.server, env.token, offset, limit, "collect")) {
            ids.append(relation.getFeedbackId());
        }
        // 获取反馈列表
        auto feedbacks = api.getFeedback(env.server, env.language, offset, limit, ids);
        return fillFeedback(api, env, feedbacks);
    });
    waitFuture(future, [this](auto resp) { emit this->signalGetCollectFeedbackResp(resp); });
}

// 获取喜欢的反馈列表
void APIProxy::getLikeFeedback(int offset, int limit)
{
    auto env = getEnv();
    auto future = QtConcurrent::run([env, offset, limit]() {
        API api(env.cachename);
        QStringList ids;
        for (auto relation : api.getFeedbackRelation(env.server, env.token, offset, limit, "like")) {
            ids.append(relation.getFeedbackId());
        }
        // 获取反馈列表
        auto feedbacks = api.getFeedback(env.server, env.language, offset, limit, ids);
        return fillFeedback(api, env, feedbacks);
    });
    waitFuture(future, [this](auto resp) { emit this->signalGetLikeFeedbackResp(resp); });
}

void APIProxy::getFeedback(QString id)
{
    auto env = getEnv();
    auto future = QtConcurrent::run([env, id]() {
        API api(env.cachename);
        // 获取反馈列表
        auto feedbacks = api.getFeedback(env.server, env.language, 0, 1, QStringList() << id);
        return fillFeedback(api, env, feedbacks);
    });
    waitFuture(future, [this](auto resp) { emit this->signalGetLikeFeedbackResp(resp); });
}

// 获取喜欢的反馈列表
void APIProxy::getUserFeedback(int offset, int limit, QString type)
{
    auto env = getEnv();
    auto future = QtConcurrent::run([env, offset, limit, type]() {
        API api(env.cachename);
        // 获取用户自己的反馈
        auto feedbacks = api.getUserFeedback(env.server, env.token, offset, limit, type);

        QList<QString> ids;
        for (auto feedback : feedbacks) {
            ids.append(feedback.getPublicId());
        }
        QHash<QString, DHHandlers_PublicStatResponse> statMap;
        for (auto stat : api.getFeedbackStat(env.server, ids)) {
            statMap.insert(stat.getPublicId(), stat);
        }

        QJsonArray arr;
        for (auto feedback : feedbacks) {
            auto obj = feedback.asJsonObject();
            auto stat = statMap[feedback.getPublicId()];
            obj["view_count"] = stat.is_view_count_Set() ? stat.getViewCount() : 0;
            obj["like_count"] = stat.is_like_count_Set() ? stat.getLikeCount() : 0;
            obj["collect_count"] = stat.is_collect_count_Set() ? stat.getCollectCount() : 0;

            QJsonArray screenshots;
            for (auto screenshot : feedback.getScreenshots()) {
                screenshots.append(env.server + "/api/v1/public/upload/" + screenshot);
            }
            obj["screenshots"] = screenshots;
            arr.append(obj);
        }
        return arr;
    });
    waitFuture(future, [this](auto resp) { emit this->signalGetUserFeedbackResp(resp); });
}
