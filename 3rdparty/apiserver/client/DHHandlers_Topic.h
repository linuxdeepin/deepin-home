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
 * DHHandlers_Topic.h
 *
 * 
 */

#ifndef DHHandlers_Topic_H
#define DHHandlers_Topic_H

#include <QJsonObject>

#include <QString>

#include "DHEnum.h"
#include "DHObject.h"

namespace DeepinHomeAPI {

class DHHandlers_Topic : public DHObject {
public:
    DHHandlers_Topic();
    DHHandlers_Topic(QString json);
    ~DHHandlers_Topic() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    QString getChangeId() const;
    void setChangeId(const QString &change_id);
    bool is_change_id_Set() const;
    bool is_change_id_Valid() const;

    QString getName() const;
    void setName(const QString &name);
    bool is_name_Set() const;
    bool is_name_Valid() const;

    bool isNotify() const;
    void setNotify(const bool &notify);
    bool is_notify_Set() const;
    bool is_notify_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    QString change_id;
    bool m_change_id_isSet;
    bool m_change_id_isValid;

    QString name;
    bool m_name_isSet;
    bool m_name_isValid;

    bool notify;
    bool m_notify_isSet;
    bool m_notify_isValid;
};

} // namespace DeepinHomeAPI

Q_DECLARE_METATYPE(DeepinHomeAPI::DHHandlers_Topic)

#endif // DHHandlers_Topic_H
