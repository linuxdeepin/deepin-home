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

#include "DHHandlers_NodeSelectResponse.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "DHHelpers.h"

namespace DeepinHomeAPI {

DHHandlers_NodeSelectResponse::DHHandlers_NodeSelectResponse(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

DHHandlers_NodeSelectResponse::DHHandlers_NodeSelectResponse() {
    this->initializeModel();
}

DHHandlers_NodeSelectResponse::~DHHandlers_NodeSelectResponse() {}

void DHHandlers_NodeSelectResponse::initializeModel() {

    m_channels_isSet = false;
    m_channels_isValid = false;

    m_refresh_time_isSet = false;
    m_refresh_time_isValid = false;

    m_server_isSet = false;
    m_server_isValid = false;
}

void DHHandlers_NodeSelectResponse::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void DHHandlers_NodeSelectResponse::fromJsonObject(QJsonObject json) {

    m_channels_isValid = ::DeepinHomeAPI::fromJsonValue(channels, json[QString("channels")]);
    m_channels_isSet = !json[QString("channels")].isNull() && m_channels_isValid;

    m_refresh_time_isValid = ::DeepinHomeAPI::fromJsonValue(refresh_time, json[QString("refresh_time")]);
    m_refresh_time_isSet = !json[QString("refresh_time")].isNull() && m_refresh_time_isValid;

    m_server_isValid = ::DeepinHomeAPI::fromJsonValue(server, json[QString("server")]);
    m_server_isSet = !json[QString("server")].isNull() && m_server_isValid;
}

QString DHHandlers_NodeSelectResponse::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject DHHandlers_NodeSelectResponse::asJsonObject() const {
    QJsonObject obj;
    if (channels.size() > 0) {
        obj.insert(QString("channels"), ::DeepinHomeAPI::toJsonValue(channels));
    }
    if (m_refresh_time_isSet) {
        obj.insert(QString("refresh_time"), ::DeepinHomeAPI::toJsonValue(refresh_time));
    }
    if (m_server_isSet) {
        obj.insert(QString("server"), ::DeepinHomeAPI::toJsonValue(server));
    }
    return obj;
}

QList<QString> DHHandlers_NodeSelectResponse::getChannels() const {
    return channels;
}
void DHHandlers_NodeSelectResponse::setChannels(const QList<QString> &channels) {
    this->channels = channels;
    this->m_channels_isSet = true;
}

bool DHHandlers_NodeSelectResponse::is_channels_Set() const{
    return m_channels_isSet;
}

bool DHHandlers_NodeSelectResponse::is_channels_Valid() const{
    return m_channels_isValid;
}

qint32 DHHandlers_NodeSelectResponse::getRefreshTime() const {
    return refresh_time;
}
void DHHandlers_NodeSelectResponse::setRefreshTime(const qint32 &refresh_time) {
    this->refresh_time = refresh_time;
    this->m_refresh_time_isSet = true;
}

bool DHHandlers_NodeSelectResponse::is_refresh_time_Set() const{
    return m_refresh_time_isSet;
}

bool DHHandlers_NodeSelectResponse::is_refresh_time_Valid() const{
    return m_refresh_time_isValid;
}

QString DHHandlers_NodeSelectResponse::getServer() const {
    return server;
}
void DHHandlers_NodeSelectResponse::setServer(const QString &server) {
    this->server = server;
    this->m_server_isSet = true;
}

bool DHHandlers_NodeSelectResponse::is_server_Set() const{
    return m_server_isSet;
}

bool DHHandlers_NodeSelectResponse::is_server_Valid() const{
    return m_server_isValid;
}

bool DHHandlers_NodeSelectResponse::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (channels.size() > 0) {
            isObjectUpdated = true;
            break;
        }

        if (m_refresh_time_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_server_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool DHHandlers_NodeSelectResponse::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace DeepinHomeAPI
