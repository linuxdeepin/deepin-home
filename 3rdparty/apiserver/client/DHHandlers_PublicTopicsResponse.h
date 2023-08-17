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
 * DHHandlers_PublicTopicsResponse.h
 *
 * 
 */

#ifndef DHHandlers_PublicTopicsResponse_H
#define DHHandlers_PublicTopicsResponse_H

#include <QJsonObject>

#include "DHHandlers_Topic.h"
#include <QList>

#include "DHEnum.h"
#include "DHObject.h"

namespace DeepinHomeAPI {
class DHHandlers_Topic;

class DHHandlers_PublicTopicsResponse : public DHObject {
public:
    DHHandlers_PublicTopicsResponse();
    DHHandlers_PublicTopicsResponse(QString json);
    ~DHHandlers_PublicTopicsResponse() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    qint32 getRefreshTime() const;
    void setRefreshTime(const qint32 &refresh_time);
    bool is_refresh_time_Set() const;
    bool is_refresh_time_Valid() const;

    QList<DHHandlers_Topic> getTopics() const;
    void setTopics(const QList<DHHandlers_Topic> &topics);
    bool is_topics_Set() const;
    bool is_topics_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    qint32 refresh_time;
    bool m_refresh_time_isSet;
    bool m_refresh_time_isValid;

    QList<DHHandlers_Topic> topics;
    bool m_topics_isSet;
    bool m_topics_isValid;
};

} // namespace DeepinHomeAPI

Q_DECLARE_METATYPE(DeepinHomeAPI::DHHandlers_PublicTopicsResponse)

#endif // DHHandlers_PublicTopicsResponse_H
