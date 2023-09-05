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
 * DHHandlers_UserRelationResposne.h
 *
 * 
 */

#ifndef DHHandlers_UserRelationResposne_H
#define DHHandlers_UserRelationResposne_H

#include <QJsonObject>

#include <QString>

#include "DHEnum.h"
#include "DHObject.h"

namespace DeepinHomeAPI {

class DHHandlers_UserRelationResposne : public DHObject {
public:
    DHHandlers_UserRelationResposne();
    DHHandlers_UserRelationResposne(QString json);
    ~DHHandlers_UserRelationResposne() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    QString getId() const;
    void setId(const QString &id);
    bool is_id_Set() const;
    bool is_id_Valid() const;

    QString getRelation() const;
    void setRelation(const QString &relation);
    bool is_relation_Set() const;
    bool is_relation_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    QString id;
    bool m_id_isSet;
    bool m_id_isValid;

    QString relation;
    bool m_relation_isSet;
    bool m_relation_isValid;
};

} // namespace DeepinHomeAPI

Q_DECLARE_METATYPE(DeepinHomeAPI::DHHandlers_UserRelationResposne)

#endif // DHHandlers_UserRelationResposne_H
