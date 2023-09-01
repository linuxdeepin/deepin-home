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

#ifndef DH_DHClientApi_H
#define DH_DHClientApi_H

#include "DHHelpers.h"
#include "DHHttpRequest.h"
#include "DHServerConfiguration.h"
#include "DHOauth.h"

#include "DHHandlers_BBSURLRequest.h"
#include "DHHandlers_BBSURLResponse.h"
#include "DHHandlers_ClientBBSTokenRequest.h"
#include "DHHandlers_ClientBBSTokenResponse.h"
#include "DHHandlers_ClientLoginRequest.h"
#include "DHHandlers_ClientLoginResponse.h"
#include "DHHandlers_ClientMessagesResponse.h"
#include "DHHandlers_ClientUserInfoResponse.h"
#include "DHHandlers_FeedbackPublicListResponse.h"
#include "DHHandlers_FeedbackUserListResponse.h"
#include "DHHandlers_FeedbackUserRelationListResponse.h"
#include "DHHandlers_LanguageCodeResponse.h"
#include "DHHandlers_LoginConfigResponse.h"
#include "DHHandlers_NodeSelectResponse.h"
#include "DHHandlers_PublicStatResponse.h"
#include "DHHandlers_PublicTopicsResponse.h"
#include <QString>

#include <QObject>
#include <QByteArray>
#include <QStringList>
#include <QList>
#include <QNetworkAccessManager>

namespace DeepinHomeAPI {

class DHClientApi : public QObject {
    Q_OBJECT

public:
    DHClientApi(const int timeOut = 0);
    ~DHClientApi();

    void initializeServerConfigs();
    int setDefaultServerValue(int serverIndex,const QString &operation, const QString &variable,const QString &val);
    void setServerIndex(const QString &operation, int serverIndex);
    void setApiKey(const QString &apiKeyName, const QString &apiKey);
    void setBearerToken(const QString &token);
    void setUsername(const QString &username);
    void setPassword(const QString &password);
    void setTimeOut(const int timeOut);
    void setWorkingDirectory(const QString &path);
    void setNetworkAccessManager(QNetworkAccessManager* manager);
    int addServerConfiguration(const QString &operation, const QUrl &url, const QString &description = "", const QMap<QString, DHServerVariable> &variables = QMap<QString, DHServerVariable>());
    void setNewServerForAllOperations(const QUrl &url, const QString &description = "", const QMap<QString, DHServerVariable> &variables =  QMap<QString, DHServerVariable>());
    void setNewServer(const QString &operation, const QUrl &url, const QString &description = "", const QMap<QString, DHServerVariable> &variables =  QMap<QString, DHServerVariable>());
    void addHeaders(const QString &key, const QString &value);
    void enableRequestCompression();
    void enableResponseCompression();
    void abortRequests();
    QString getParamStylePrefix(const QString &style);
    QString getParamStyleSuffix(const QString &style);
    QString getParamStyleDelimiter(const QString &style, const QString &name, bool isExplode);

    /**
    * @param[in]  data DHHandlers_ClientLoginRequest [required]
    */
    void clientLogin(const DHHandlers_ClientLoginRequest &data);

    /**
    * @param[in]  data DHHandlers_ClientBBSTokenRequest [required]
    */
    void getBBSToken(const DHHandlers_ClientBBSTokenRequest &data);

    /**
    * @param[in]  data DHHandlers_BBSURLRequest [required]
    */
    void getBBSURL(const DHHandlers_BBSURLRequest &data);

    /**
    * @param[in]  language QString [required]
    * @param[in]  offset double [required]
    * @param[in]  limit double [required]
    * @param[in]  type QString [optional]
    * @param[in]  status QString [optional]
    * @param[in]  public_id QList<QString> [optional]
    */
    void getFeedback(const QString &language, const double &offset, const double &limit, const ::DeepinHomeAPI::OptionalParam<QString> &type = ::DeepinHomeAPI::OptionalParam<QString>(), const ::DeepinHomeAPI::OptionalParam<QString> &status = ::DeepinHomeAPI::OptionalParam<QString>(), const ::DeepinHomeAPI::OptionalParam<QList<QString>> &public_id = ::DeepinHomeAPI::OptionalParam<QList<QString>>());

    /**
    * @param[in]  offset double [required]
    * @param[in]  limit double [required]
    * @param[in]  id QList<QString> [optional]
    * @param[in]  relation QList<QString> [optional]
    */
    void getFeedbackRelation(const double &offset, const double &limit, const ::DeepinHomeAPI::OptionalParam<QList<QString>> &id = ::DeepinHomeAPI::OptionalParam<QList<QString>>(), const ::DeepinHomeAPI::OptionalParam<QList<QString>> &relation = ::DeepinHomeAPI::OptionalParam<QList<QString>>());

    /**
    * @param[in]  id QList<QString> [required]
    */
    void getFeedbackStat(const QList<QString> &id);

    /**
    * @param[in]  code QString [required]
    */
    void getLanguageCode(const QString &code);


    void getLoginConfig();


    void getLoginInfo();

    /**
    * @param[in]  channel_id QString [required]
    * @param[in]  topic_id QString [required]
    * @param[in]  language QString [required]
    */
    void getMessages(const QString &channel_id, const QString &topic_id, const QString &language);

    /**
    * @param[in]  machine_id QString [required]
    */
    void getNodes(const QString &machine_id);

    /**
    * @param[in]  channel_id QString [required]
    */
    void getTopics(const QString &channel_id);

    /**
    * @param[in]  offset double [required]
    * @param[in]  limit double [required]
    * @param[in]  type QString [optional]
    * @param[in]  status QString [optional]
    */
    void getUserFeedback(const double &offset, const double &limit, const ::DeepinHomeAPI::OptionalParam<QString> &type = ::DeepinHomeAPI::OptionalParam<QString>(), const ::DeepinHomeAPI::OptionalParam<QString> &status = ::DeepinHomeAPI::OptionalParam<QString>());


private:
    QMap<QString,int> _serverIndices;
    QMap<QString,QList<DHServerConfiguration>> _serverConfigs;
    QMap<QString, QString> _apiKeys;
    QString _bearerToken;
    QString _username;
    QString _password;
    int _timeOut;
    QString _workingDirectory;
    QNetworkAccessManager* _manager;
    QMap<QString, QString> _defaultHeaders;
    bool _isResponseCompressionEnabled;
    bool _isRequestCompressionEnabled;
    DHHttpRequestInput _latestInput;
    DHHttpRequestWorker *_latestWorker;
    QStringList _latestScope;
    OauthCode _authFlow;
    OauthImplicit _implicitFlow;
    OauthCredentials _credentialFlow;
    OauthPassword _passwordFlow;
    int _OauthMethod = 0;

    void clientLoginCallback(DHHttpRequestWorker *worker);
    void getBBSTokenCallback(DHHttpRequestWorker *worker);
    void getBBSURLCallback(DHHttpRequestWorker *worker);
    void getFeedbackCallback(DHHttpRequestWorker *worker);
    void getFeedbackRelationCallback(DHHttpRequestWorker *worker);
    void getFeedbackStatCallback(DHHttpRequestWorker *worker);
    void getLanguageCodeCallback(DHHttpRequestWorker *worker);
    void getLoginConfigCallback(DHHttpRequestWorker *worker);
    void getLoginInfoCallback(DHHttpRequestWorker *worker);
    void getMessagesCallback(DHHttpRequestWorker *worker);
    void getNodesCallback(DHHttpRequestWorker *worker);
    void getTopicsCallback(DHHttpRequestWorker *worker);
    void getUserFeedbackCallback(DHHttpRequestWorker *worker);

signals:

    void clientLoginSignal(DHHandlers_ClientLoginResponse summary);
    void getBBSTokenSignal(DHHandlers_ClientBBSTokenResponse summary);
    void getBBSURLSignal(DHHandlers_BBSURLResponse summary);
    void getFeedbackSignal(QList<DHHandlers_FeedbackPublicListResponse> summary);
    void getFeedbackRelationSignal(QList<DHHandlers_FeedbackUserRelationListResponse> summary);
    void getFeedbackStatSignal(QList<DHHandlers_PublicStatResponse> summary);
    void getLanguageCodeSignal(DHHandlers_LanguageCodeResponse summary);
    void getLoginConfigSignal(DHHandlers_LoginConfigResponse summary);
    void getLoginInfoSignal(DHHandlers_ClientUserInfoResponse summary);
    void getMessagesSignal(QList<DHHandlers_ClientMessagesResponse> summary);
    void getNodesSignal(DHHandlers_NodeSelectResponse summary);
    void getTopicsSignal(DHHandlers_PublicTopicsResponse summary);
    void getUserFeedbackSignal(QList<DHHandlers_FeedbackUserListResponse> summary);

    void clientLoginSignalFull(DHHttpRequestWorker *worker, DHHandlers_ClientLoginResponse summary);
    void getBBSTokenSignalFull(DHHttpRequestWorker *worker, DHHandlers_ClientBBSTokenResponse summary);
    void getBBSURLSignalFull(DHHttpRequestWorker *worker, DHHandlers_BBSURLResponse summary);
    void getFeedbackSignalFull(DHHttpRequestWorker *worker, QList<DHHandlers_FeedbackPublicListResponse> summary);
    void getFeedbackRelationSignalFull(DHHttpRequestWorker *worker, QList<DHHandlers_FeedbackUserRelationListResponse> summary);
    void getFeedbackStatSignalFull(DHHttpRequestWorker *worker, QList<DHHandlers_PublicStatResponse> summary);
    void getLanguageCodeSignalFull(DHHttpRequestWorker *worker, DHHandlers_LanguageCodeResponse summary);
    void getLoginConfigSignalFull(DHHttpRequestWorker *worker, DHHandlers_LoginConfigResponse summary);
    void getLoginInfoSignalFull(DHHttpRequestWorker *worker, DHHandlers_ClientUserInfoResponse summary);
    void getMessagesSignalFull(DHHttpRequestWorker *worker, QList<DHHandlers_ClientMessagesResponse> summary);
    void getNodesSignalFull(DHHttpRequestWorker *worker, DHHandlers_NodeSelectResponse summary);
    void getTopicsSignalFull(DHHttpRequestWorker *worker, DHHandlers_PublicTopicsResponse summary);
    void getUserFeedbackSignalFull(DHHttpRequestWorker *worker, QList<DHHandlers_FeedbackUserListResponse> summary);

    void clientLoginSignalE(DHHandlers_ClientLoginResponse summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getBBSTokenSignalE(DHHandlers_ClientBBSTokenResponse summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getBBSURLSignalE(DHHandlers_BBSURLResponse summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getFeedbackSignalE(QList<DHHandlers_FeedbackPublicListResponse> summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getFeedbackRelationSignalE(QList<DHHandlers_FeedbackUserRelationListResponse> summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getFeedbackStatSignalE(QList<DHHandlers_PublicStatResponse> summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getLanguageCodeSignalE(DHHandlers_LanguageCodeResponse summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getLoginConfigSignalE(DHHandlers_LoginConfigResponse summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getLoginInfoSignalE(DHHandlers_ClientUserInfoResponse summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getMessagesSignalE(QList<DHHandlers_ClientMessagesResponse> summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getNodesSignalE(DHHandlers_NodeSelectResponse summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getTopicsSignalE(DHHandlers_PublicTopicsResponse summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getUserFeedbackSignalE(QList<DHHandlers_FeedbackUserListResponse> summary, QNetworkReply::NetworkError error_type, QString error_str);

    void clientLoginSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getBBSTokenSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getBBSURLSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getFeedbackSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getFeedbackRelationSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getFeedbackStatSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getLanguageCodeSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getLoginConfigSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getLoginInfoSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getMessagesSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getNodesSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getTopicsSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getUserFeedbackSignalEFull(DHHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);

    void abortRequestsSignal();
    void allPendingRequestsCompleted();

public slots:
    void tokenAvailable();
    
};

} // namespace DeepinHomeAPI
#endif
