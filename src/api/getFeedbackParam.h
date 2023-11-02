// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <DHClientApi.h>
#include <QJsonObject>

using namespace DeepinHomeAPI;

namespace SyncAPI {
class GetFeedbackOptionalParam
{
public:
    OptionalParam<QString> type;
    OptionalParam<QList<QString>> status;
    OptionalParam<QList<QString>> public_id;
    OptionalParam<QString> order;
    OptionalParam<QString> version;
    OptionalParam<QString> keyword;
    GetFeedbackOptionalParam(QJsonObject obj){
        fromJsonObject(obj);
    };
    void fromJsonObject(QJsonObject obj)
    {
        if (obj["type"].toString().length() > 0) {
            type = obj["type"].toString();
        }
        if (obj["status"].toArray().size() > 0) {
            status = obj["status"].toVariant().toStringList();
        }
        if (obj["public_id"].toArray().size() > 0) {
            public_id = obj["public_id"].toVariant().toStringList();
        }
        if (obj["order"].toString().length() > 0) {
            order = obj["order"].toString();
        }
        if (obj["version"].toString().length() > 0) {
            version = obj["version"].toString();
        }
        if (obj["keyword"].toString().length() > 0) {
            keyword = obj["keyword"].toString();
        }
    };
};
} // namespace SyncAPI