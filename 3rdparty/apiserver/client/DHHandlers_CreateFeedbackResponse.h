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
 * DHHandlers_CreateFeedbackResponse.h
 *
 * 
 */

#ifndef DHHandlers_CreateFeedbackResponse_H
#define DHHandlers_CreateFeedbackResponse_H

#include <QJsonObject>

#include <QString>

#include "DHEnum.h"
#include "DHObject.h"

namespace DeepinHomeAPI {

class DHHandlers_CreateFeedbackResponse : public DHObject {
public:
    DHHandlers_CreateFeedbackResponse();
    DHHandlers_CreateFeedbackResponse(QString json);
    ~DHHandlers_CreateFeedbackResponse() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    QString getPublicId() const;
    void setPublicId(const QString &public_id);
    bool is_public_id_Set() const;
    bool is_public_id_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    QString public_id;
    bool m_public_id_isSet;
    bool m_public_id_isValid;
};

} // namespace DeepinHomeAPI

Q_DECLARE_METATYPE(DeepinHomeAPI::DHHandlers_CreateFeedbackResponse)

#endif // DHHandlers_CreateFeedbackResponse_H