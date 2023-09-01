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

#include "DHHandlers_FeedbackUserListResponse.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "DHHelpers.h"

namespace DeepinHomeAPI {

DHHandlers_FeedbackUserListResponse::DHHandlers_FeedbackUserListResponse(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

DHHandlers_FeedbackUserListResponse::DHHandlers_FeedbackUserListResponse() {
    this->initializeModel();
}

DHHandlers_FeedbackUserListResponse::~DHHandlers_FeedbackUserListResponse() {}

void DHHandlers_FeedbackUserListResponse::initializeModel() {

    m_avatar_isSet = false;
    m_avatar_isValid = false;

    m_content_isSet = false;
    m_content_isValid = false;

    m_created_at_isSet = false;
    m_created_at_isValid = false;

    m_module_id_isSet = false;
    m_module_id_isValid = false;

    m_nickname_isSet = false;
    m_nickname_isValid = false;

    m_public_id_isSet = false;
    m_public_id_isValid = false;

    m_screenshots_isSet = false;
    m_screenshots_isValid = false;

    m_status_isSet = false;
    m_status_isValid = false;

    m_system_version_isSet = false;
    m_system_version_isValid = false;

    m_title_isSet = false;
    m_title_isValid = false;

    m_type_isSet = false;
    m_type_isValid = false;

    m_updated_at_isSet = false;
    m_updated_at_isValid = false;

    m_username_isSet = false;
    m_username_isValid = false;
}

void DHHandlers_FeedbackUserListResponse::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void DHHandlers_FeedbackUserListResponse::fromJsonObject(QJsonObject json) {

    m_avatar_isValid = ::DeepinHomeAPI::fromJsonValue(avatar, json[QString("avatar")]);
    m_avatar_isSet = !json[QString("avatar")].isNull() && m_avatar_isValid;

    m_content_isValid = ::DeepinHomeAPI::fromJsonValue(content, json[QString("content")]);
    m_content_isSet = !json[QString("content")].isNull() && m_content_isValid;

    m_created_at_isValid = ::DeepinHomeAPI::fromJsonValue(created_at, json[QString("created_at")]);
    m_created_at_isSet = !json[QString("created_at")].isNull() && m_created_at_isValid;

    m_module_id_isValid = ::DeepinHomeAPI::fromJsonValue(module_id, json[QString("module_id")]);
    m_module_id_isSet = !json[QString("module_id")].isNull() && m_module_id_isValid;

    m_nickname_isValid = ::DeepinHomeAPI::fromJsonValue(nickname, json[QString("nickname")]);
    m_nickname_isSet = !json[QString("nickname")].isNull() && m_nickname_isValid;

    m_public_id_isValid = ::DeepinHomeAPI::fromJsonValue(public_id, json[QString("public_id")]);
    m_public_id_isSet = !json[QString("public_id")].isNull() && m_public_id_isValid;

    m_screenshots_isValid = ::DeepinHomeAPI::fromJsonValue(screenshots, json[QString("screenshots")]);
    m_screenshots_isSet = !json[QString("screenshots")].isNull() && m_screenshots_isValid;

    m_status_isValid = ::DeepinHomeAPI::fromJsonValue(status, json[QString("status")]);
    m_status_isSet = !json[QString("status")].isNull() && m_status_isValid;

    m_system_version_isValid = ::DeepinHomeAPI::fromJsonValue(system_version, json[QString("system_version")]);
    m_system_version_isSet = !json[QString("system_version")].isNull() && m_system_version_isValid;

    m_title_isValid = ::DeepinHomeAPI::fromJsonValue(title, json[QString("title")]);
    m_title_isSet = !json[QString("title")].isNull() && m_title_isValid;

    m_type_isValid = ::DeepinHomeAPI::fromJsonValue(type, json[QString("type")]);
    m_type_isSet = !json[QString("type")].isNull() && m_type_isValid;

    m_updated_at_isValid = ::DeepinHomeAPI::fromJsonValue(updated_at, json[QString("updated_at")]);
    m_updated_at_isSet = !json[QString("updated_at")].isNull() && m_updated_at_isValid;

    m_username_isValid = ::DeepinHomeAPI::fromJsonValue(username, json[QString("username")]);
    m_username_isSet = !json[QString("username")].isNull() && m_username_isValid;
}

QString DHHandlers_FeedbackUserListResponse::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject DHHandlers_FeedbackUserListResponse::asJsonObject() const {
    QJsonObject obj;
    if (m_avatar_isSet) {
        obj.insert(QString("avatar"), ::DeepinHomeAPI::toJsonValue(avatar));
    }
    if (m_content_isSet) {
        obj.insert(QString("content"), ::DeepinHomeAPI::toJsonValue(content));
    }
    if (m_created_at_isSet) {
        obj.insert(QString("created_at"), ::DeepinHomeAPI::toJsonValue(created_at));
    }
    if (m_module_id_isSet) {
        obj.insert(QString("module_id"), ::DeepinHomeAPI::toJsonValue(module_id));
    }
    if (m_nickname_isSet) {
        obj.insert(QString("nickname"), ::DeepinHomeAPI::toJsonValue(nickname));
    }
    if (m_public_id_isSet) {
        obj.insert(QString("public_id"), ::DeepinHomeAPI::toJsonValue(public_id));
    }
    if (screenshots.size() > 0) {
        obj.insert(QString("screenshots"), ::DeepinHomeAPI::toJsonValue(screenshots));
    }
    if (m_status_isSet) {
        obj.insert(QString("status"), ::DeepinHomeAPI::toJsonValue(status));
    }
    if (m_system_version_isSet) {
        obj.insert(QString("system_version"), ::DeepinHomeAPI::toJsonValue(system_version));
    }
    if (m_title_isSet) {
        obj.insert(QString("title"), ::DeepinHomeAPI::toJsonValue(title));
    }
    if (m_type_isSet) {
        obj.insert(QString("type"), ::DeepinHomeAPI::toJsonValue(type));
    }
    if (m_updated_at_isSet) {
        obj.insert(QString("updated_at"), ::DeepinHomeAPI::toJsonValue(updated_at));
    }
    if (m_username_isSet) {
        obj.insert(QString("username"), ::DeepinHomeAPI::toJsonValue(username));
    }
    return obj;
}

QString DHHandlers_FeedbackUserListResponse::getAvatar() const {
    return avatar;
}
void DHHandlers_FeedbackUserListResponse::setAvatar(const QString &avatar) {
    this->avatar = avatar;
    this->m_avatar_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_avatar_Set() const{
    return m_avatar_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_avatar_Valid() const{
    return m_avatar_isValid;
}

QString DHHandlers_FeedbackUserListResponse::getContent() const {
    return content;
}
void DHHandlers_FeedbackUserListResponse::setContent(const QString &content) {
    this->content = content;
    this->m_content_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_content_Set() const{
    return m_content_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_content_Valid() const{
    return m_content_isValid;
}

QString DHHandlers_FeedbackUserListResponse::getCreatedAt() const {
    return created_at;
}
void DHHandlers_FeedbackUserListResponse::setCreatedAt(const QString &created_at) {
    this->created_at = created_at;
    this->m_created_at_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_created_at_Set() const{
    return m_created_at_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_created_at_Valid() const{
    return m_created_at_isValid;
}

qint32 DHHandlers_FeedbackUserListResponse::getModuleId() const {
    return module_id;
}
void DHHandlers_FeedbackUserListResponse::setModuleId(const qint32 &module_id) {
    this->module_id = module_id;
    this->m_module_id_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_module_id_Set() const{
    return m_module_id_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_module_id_Valid() const{
    return m_module_id_isValid;
}

QString DHHandlers_FeedbackUserListResponse::getNickname() const {
    return nickname;
}
void DHHandlers_FeedbackUserListResponse::setNickname(const QString &nickname) {
    this->nickname = nickname;
    this->m_nickname_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_nickname_Set() const{
    return m_nickname_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_nickname_Valid() const{
    return m_nickname_isValid;
}

QString DHHandlers_FeedbackUserListResponse::getPublicId() const {
    return public_id;
}
void DHHandlers_FeedbackUserListResponse::setPublicId(const QString &public_id) {
    this->public_id = public_id;
    this->m_public_id_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_public_id_Set() const{
    return m_public_id_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_public_id_Valid() const{
    return m_public_id_isValid;
}

QList<QString> DHHandlers_FeedbackUserListResponse::getScreenshots() const {
    return screenshots;
}
void DHHandlers_FeedbackUserListResponse::setScreenshots(const QList<QString> &screenshots) {
    this->screenshots = screenshots;
    this->m_screenshots_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_screenshots_Set() const{
    return m_screenshots_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_screenshots_Valid() const{
    return m_screenshots_isValid;
}

QString DHHandlers_FeedbackUserListResponse::getStatus() const {
    return status;
}
void DHHandlers_FeedbackUserListResponse::setStatus(const QString &status) {
    this->status = status;
    this->m_status_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_status_Set() const{
    return m_status_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_status_Valid() const{
    return m_status_isValid;
}

QString DHHandlers_FeedbackUserListResponse::getSystemVersion() const {
    return system_version;
}
void DHHandlers_FeedbackUserListResponse::setSystemVersion(const QString &system_version) {
    this->system_version = system_version;
    this->m_system_version_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_system_version_Set() const{
    return m_system_version_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_system_version_Valid() const{
    return m_system_version_isValid;
}

QString DHHandlers_FeedbackUserListResponse::getTitle() const {
    return title;
}
void DHHandlers_FeedbackUserListResponse::setTitle(const QString &title) {
    this->title = title;
    this->m_title_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_title_Set() const{
    return m_title_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_title_Valid() const{
    return m_title_isValid;
}

QString DHHandlers_FeedbackUserListResponse::getType() const {
    return type;
}
void DHHandlers_FeedbackUserListResponse::setType(const QString &type) {
    this->type = type;
    this->m_type_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_type_Set() const{
    return m_type_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_type_Valid() const{
    return m_type_isValid;
}

QString DHHandlers_FeedbackUserListResponse::getUpdatedAt() const {
    return updated_at;
}
void DHHandlers_FeedbackUserListResponse::setUpdatedAt(const QString &updated_at) {
    this->updated_at = updated_at;
    this->m_updated_at_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_updated_at_Set() const{
    return m_updated_at_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_updated_at_Valid() const{
    return m_updated_at_isValid;
}

QString DHHandlers_FeedbackUserListResponse::getUsername() const {
    return username;
}
void DHHandlers_FeedbackUserListResponse::setUsername(const QString &username) {
    this->username = username;
    this->m_username_isSet = true;
}

bool DHHandlers_FeedbackUserListResponse::is_username_Set() const{
    return m_username_isSet;
}

bool DHHandlers_FeedbackUserListResponse::is_username_Valid() const{
    return m_username_isValid;
}

bool DHHandlers_FeedbackUserListResponse::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_avatar_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_content_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_created_at_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_module_id_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_nickname_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_public_id_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (screenshots.size() > 0) {
            isObjectUpdated = true;
            break;
        }

        if (m_status_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_system_version_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_title_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_type_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_updated_at_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_username_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool DHHandlers_FeedbackUserListResponse::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace DeepinHomeAPI
