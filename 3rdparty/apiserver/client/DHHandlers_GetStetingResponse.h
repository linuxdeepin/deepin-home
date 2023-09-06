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
 * DHHandlers_GetStetingResponse.h
 *
 * 
 */

#ifndef DHHandlers_GetStetingResponse_H
#define DHHandlers_GetStetingResponse_H

#include <QJsonObject>

#include <QString>

#include "DHEnum.h"
#include "DHObject.h"

namespace DeepinHomeAPI {

class DHHandlers_GetStetingResponse : public DHObject {
public:
    DHHandlers_GetStetingResponse();
    DHHandlers_GetStetingResponse(QString json);
    ~DHHandlers_GetStetingResponse() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    QString getKey() const;
    void setKey(const QString &key);
    bool is_key_Set() const;
    bool is_key_Valid() const;

    QString getValue() const;
    void setValue(const QString &value);
    bool is_value_Set() const;
    bool is_value_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    QString key;
    bool m_key_isSet;
    bool m_key_isValid;

    QString value;
    bool m_value_isSet;
    bool m_value_isValid;
};

} // namespace DeepinHomeAPI

Q_DECLARE_METATYPE(DeepinHomeAPI::DHHandlers_GetStetingResponse)

#endif // DHHandlers_GetStetingResponse_H