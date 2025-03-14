#ifndef HTTPMGR_H
#define HTTPMGR_H
#include"singleton.h"
#include"global.h"
#include<QString>
#include<QUrl>
#include<QObject>
#include<QNetworkAccessManager>
#include<QJsonObject>
#include<QJsonDocument>
class HttpMgr : public QObject,public Singleton<HttpMgr>,public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT
    friend class RegusterDialog;
public:
    ~HttpMgr();
private:
    friend class Singleton<HttpMgr>;
    HttpMgr();
    QNetworkAccessManager _manager;
    void PostHttpReq(QUrl url , QJsonObject json,ReqId req_id,Modules mod);
private slots:
    void slot_http_finish(ReqId id,QString res,ErrorCodes err,Modules mod);
signals:
    void sig_http_finish(ReqId id,QString res,ErrorCodes err,Modules mod);
    void sig_reg_mod_finish(ReqId id,QString res,ErrorCodes err);
};

#endif // HTTPMGR_H
