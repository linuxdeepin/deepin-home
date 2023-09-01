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

/*
 * DHHandlers_FeedbackUserListResponse.h
 *
 * 
 */

#ifndef DHHandlers_FeedbackUserListResponse_H
#define DHHandlers_FeedbackUserListResponse_H

#include <QJsonObject>

#include <QList>
#include <QString>

#include "DHEnum.h"
#include "DHObject.h"

namespace DeepinHomeAPI {

class DHHandlers_FeedbackUserListResponse : public DHObject {
public:
    DHHandlers_FeedbackUserListResponse();
    DHHandlers_FeedbackUserListResponse(QString json);
    ~DHHandlers_FeedbackUserListResponse() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    QString getAvatar() const;
    void setAvatar(const QString &avatar);
    bool is_avatar_Set() const;
    bool is_avatar_Valid() const;

    QString getContent() const;
    void setContent(const QString &content);
    bool is_content_Set() const;
    bool is_content_Valid() const;

    QString getCreatedAt() const;
    void setCreatedAt(const QString &created_at);
    bool is_created_at_Set() const;
    bool is_created_at_Valid() const;

    qint32 getModuleId() const;
    void setModuleId(const qint32 &module_id);
    bool is_module_id_Set() const;
    bool is_module_id_Valid() const;

    QString getNickname() const;
    void setNickname(const QString &nickname);
    bool is_nickname_Set() const;
    bool is_nickname_Valid() const;

    QString getPublicId() const;
    void setPublicId(const QString &public_id);
    bool is_public_id_Set() const;
    bool is_public_id_Valid() const;

    QList<QString> getScreenshots() const;
    void setScreenshots(const QList<QString> &screenshots);
    bool is_screenshots_Set() const;
    bool is_screenshots_Valid() const;

    QString getStatus() const;
    void setStatus(const QString &status);
    bool is_status_Set() const;
    bool is_status_Valid() const;

    QString getSystemVersion() const;
    void setSystemVersion(const QString &system_version);
    bool is_system_version_Set() const;
    bool is_system_version_Valid() const;

    QString getTitle() const;
    void setTitle(const QString &title);
    bool is_title_Set() const;
    bool is_title_Valid() const;

    QString getType() const;
    void setType(const QString &type);
    bool is_type_Set() const;
    bool is_type_Valid() const;

    QString getUpdatedAt() const;
    void setUpdatedAt(const QString &updated_at);
    bool is_updated_at_Set() const;
    bool is_updated_at_Valid() const;

    QString getUsername() const;
    void setUsername(const QString &username);
    bool is_username_Set() const;
    bool is_username_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    QString avatar;
    bool m_avatar_isSet;
    bool m_avatar_isValid;

    QString content;
    bool m_content_isSet;
    bool m_content_isValid;

    QString created_at;
    bool m_created_at_isSet;
    bool m_created_at_isValid;

    qint32 module_id;
    bool m_module_id_isSet;
    bool m_module_id_isValid;

    QString nickname;
    bool m_nickname_isSet;
    bool m_nickname_isValid;

    QString public_id;
    bool m_public_id_isSet;
    bool m_public_id_isValid;

    QList<QString> screenshots;
    bool m_screenshots_isSet;
    bool m_screenshots_isValid;

    QString status;
    bool m_status_isSet;
    bool m_status_isValid;

    QString system_version;
    bool m_system_version_isSet;
    bool m_system_version_isValid;

    QString title;
    bool m_title_isSet;
    bool m_title_isValid;

    QString type;
    bool m_type_isSet;
    bool m_type_isValid;

    QString updated_at;
    bool m_updated_at_isSet;
    bool m_updated_at_isValid;

    QString username;
    bool m_username_isSet;
    bool m_username_isValid;
};

} // namespace DeepinHomeAPI

Q_DECLARE_METATYPE(DeepinHomeAPI::DHHandlers_FeedbackUserListResponse)

#endif // DHHandlers_FeedbackUserListResponse_H
