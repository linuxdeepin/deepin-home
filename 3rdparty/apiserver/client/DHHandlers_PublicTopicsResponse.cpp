/**
 * Deepin官网
 * deepin的官方网站后台接口
 *
 * The version of the OpenAPI document: 1.0
 * Contact: wurongjie@deepin.org
 *
 * NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

#include "DHHandlers_PublicTopicsResponse.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "DHHelpers.h"

namespace DeepinHomeAPI {

DHHandlers_PublicTopicsResponse::DHHandlers_PublicTopicsResponse(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

DHHandlers_PublicTopicsResponse::DHHandlers_PublicTopicsResponse() {
    this->initializeModel();
}

DHHandlers_PublicTopicsResponse::~DHHandlers_PublicTopicsResponse() {}

void DHHandlers_PublicTopicsResponse::initializeModel() {

    m_refresh_time_isSet = false;
    m_refresh_time_isValid = false;

    m_topics_isSet = false;
    m_topics_isValid = false;
}

void DHHandlers_PublicTopicsResponse::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void DHHandlers_PublicTopicsResponse::fromJsonObject(QJsonObject json) {

    m_refresh_time_isValid = ::DeepinHomeAPI::fromJsonValue(refresh_time, json[QString("refresh_time")]);
    m_refresh_time_isSet = !json[QString("refresh_time")].isNull() && m_refresh_time_isValid;

    m_topics_isValid = ::DeepinHomeAPI::fromJsonValue(topics, json[QString("topics")]);
    m_topics_isSet = !json[QString("topics")].isNull() && m_topics_isValid;
}

QString DHHandlers_PublicTopicsResponse::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject DHHandlers_PublicTopicsResponse::asJsonObject() const {
    QJsonObject obj;
    if (m_refresh_time_isSet) {
        obj.insert(QString("refresh_time"), ::DeepinHomeAPI::toJsonValue(refresh_time));
    }
    if (topics.size() > 0) {
        obj.insert(QString("topics"), ::DeepinHomeAPI::toJsonValue(topics));
    }
    return obj;
}

qint32 DHHandlers_PublicTopicsResponse::getRefreshTime() const {
    return refresh_time;
}
void DHHandlers_PublicTopicsResponse::setRefreshTime(const qint32 &refresh_time) {
    this->refresh_time = refresh_time;
    this->m_refresh_time_isSet = true;
}

bool DHHandlers_PublicTopicsResponse::is_refresh_time_Set() const{
    return m_refresh_time_isSet;
}

bool DHHandlers_PublicTopicsResponse::is_refresh_time_Valid() const{
    return m_refresh_time_isValid;
}

QList<DHHandlers_Topic> DHHandlers_PublicTopicsResponse::getTopics() const {
    return topics;
}
void DHHandlers_PublicTopicsResponse::setTopics(const QList<DHHandlers_Topic> &topics) {
    this->topics = topics;
    this->m_topics_isSet = true;
}

bool DHHandlers_PublicTopicsResponse::is_topics_Set() const{
    return m_topics_isSet;
}

bool DHHandlers_PublicTopicsResponse::is_topics_Valid() const{
    return m_topics_isValid;
}

bool DHHandlers_PublicTopicsResponse::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_refresh_time_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (topics.size() > 0) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool DHHandlers_PublicTopicsResponse::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace DeepinHomeAPI
