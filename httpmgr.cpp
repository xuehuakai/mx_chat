#include "httpmgr.h"
#include<QPointer>
#include<QMessageBox>
HttpMgr::~HttpMgr()
{

}

HttpMgr::HttpMgr() {
    connect(this,&HttpMgr::sig_http_finish,this,&HttpMgr::slot_http_finish);
}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(data.length()));
    auto self = shared_from_this();
    QNetworkReply * reply = _manager.post(request,data);
    QObject::connect(reply,&QNetworkReply::finished,this,[self,reply,req_id,mod](){

        if(reply->error() != QNetworkReply::NoError){
            qDebug()<<reply->errorString();
            //发送信号通知完成
            emit self->sig_http_finish(req_id,"",ErrorCodes::ERR_NETWORK,mod);
            reply->deleteLater();
            return;
        }
        //no error
        QString res = reply->readAll();
        //发送信号通知完成
        emit self->sig_http_finish(req_id,res,ErrorCodes::SUCCESS,mod);
        reply->deleteLater();
        return;
    });
}

void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if(mod == Modules::REGISTERMOD){//注册模块
        //发送注册信息通知指定模块http的响应结束了
        emit sig_reg_mod_finish(id,res,err);
    }

    if(mod == Modules::RESETMOD){
        emit sig_reset_mod_finish(id,res,err);
    }
}
