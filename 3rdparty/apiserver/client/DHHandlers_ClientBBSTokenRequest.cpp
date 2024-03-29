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

#include "DHHandlers_ClientBBSTokenRequest.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "DHHelpers.h"

namespace DeepinHomeAPI {

DHHandlers_ClientBBSTokenRequest::DHHandlers_ClientBBSTokenRequest(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

DHHandlers_ClientBBSTokenRequest::DHHandlers_ClientBBSTokenRequest() {
    this->initializeModel();
}

DHHandlers_ClientBBSTokenRequest::~DHHandlers_ClientBBSTokenRequest() {}

void DHHandlers_ClientBBSTokenRequest::initializeModel() {

    m_id_isSet = false;
    m_id_isValid = false;

    m_secret_isSet = false;
    m_secret_isValid = false;
}

void DHHandlers_ClientBBSTokenRequest::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void DHHandlers_ClientBBSTokenRequest::fromJsonObject(QJsonObject json) {

    m_id_isValid = ::DeepinHomeAPI::fromJsonValue(id, json[QString("id")]);
    m_id_isSet = !json[QString("id")].isNull() && m_id_isValid;

    m_secret_isValid = ::DeepinHomeAPI::fromJsonValue(secret, json[QString("secret")]);
    m_secret_isSet = !json[QString("secret")].isNull() && m_secret_isValid;
}

QString DHHandlers_ClientBBSTokenRequest::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject DHHandlers_ClientBBSTokenRequest::asJsonObject() const {
    QJsonObject obj;
    if (m_id_isSet) {
        obj.insert(QString("id"), ::DeepinHomeAPI::toJsonValue(id));
    }
    if (m_secret_isSet) {
        obj.insert(QString("secret"), ::DeepinHomeAPI::toJsonValue(secret));
    }
    return obj;
}

QString DHHandlers_ClientBBSTokenRequest::getId() const {
    return id;
}
void DHHandlers_ClientBBSTokenRequest::setId(const QString &id) {
    this->id = id;
    this->m_id_isSet = true;
}

bool DHHandlers_ClientBBSTokenRequest::is_id_Set() const{
    return m_id_isSet;
}

bool DHHandlers_ClientBBSTokenRequest::is_id_Valid() const{
    return m_id_isValid;
}

QString DHHandlers_ClientBBSTokenRequest::getSecret() const {
    return secret;
}
void DHHandlers_ClientBBSTokenRequest::setSecret(const QString &secret) {
    this->secret = secret;
    this->m_secret_isSet = true;
}

bool DHHandlers_ClientBBSTokenRequest::is_secret_Set() const{
    return m_secret_isSet;
}

bool DHHandlers_ClientBBSTokenRequest::is_secret_Valid() const{
    return m_secret_isValid;
}

bool DHHandlers_ClientBBSTokenRequest::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_id_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_secret_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool DHHandlers_ClientBBSTokenRequest::isValid() const {
    // only required properties are required for the object to be considered valid
    return m_id_isValid && m_secret_isValid && true;
}

} // namespace DeepinHomeAPI
