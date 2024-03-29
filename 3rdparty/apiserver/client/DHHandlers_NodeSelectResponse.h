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
 * DHHandlers_NodeSelectResponse.h
 *
 * 
 */

#ifndef DHHandlers_NodeSelectResponse_H
#define DHHandlers_NodeSelectResponse_H

#include <QJsonObject>

#include <QList>
#include <QString>

#include "DHEnum.h"
#include "DHObject.h"

namespace DeepinHomeAPI {

class DHHandlers_NodeSelectResponse : public DHObject {
public:
    DHHandlers_NodeSelectResponse();
    DHHandlers_NodeSelectResponse(QString json);
    ~DHHandlers_NodeSelectResponse() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    QList<QString> getChannels() const;
    void setChannels(const QList<QString> &channels);
    bool is_channels_Set() const;
    bool is_channels_Valid() const;

    qint32 getRefreshTime() const;
    void setRefreshTime(const qint32 &refresh_time);
    bool is_refresh_time_Set() const;
    bool is_refresh_time_Valid() const;

    QString getServer() const;
    void setServer(const QString &server);
    bool is_server_Set() const;
    bool is_server_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    QList<QString> channels;
    bool m_channels_isSet;
    bool m_channels_isValid;

    qint32 refresh_time;
    bool m_refresh_time_isSet;
    bool m_refresh_time_isValid;

    QString server;
    bool m_server_isSet;
    bool m_server_isValid;
};

} // namespace DeepinHomeAPI

Q_DECLARE_METATYPE(DeepinHomeAPI::DHHandlers_NodeSelectResponse)

#endif // DHHandlers_NodeSelectResponse_H
