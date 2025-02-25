#include "regusterdialog.h"
#include "ui_regusterdialog.h"
#include"global.h"
#include"httpmgr.h"
RegusterDialog::RegusterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegusterDialog)
{
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);
    ui->err_trip->setProperty("state","normal");
    repolish(ui->err_trip);
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_reg_mod_finish,
            this,&RegusterDialog::slot_reg_mod_finish);

    initHttpHandles();
}

RegusterDialog::~RegusterDialog()
{
    delete ui;
}

void RegusterDialog::on_get_code_btn_clicked()
{
    QString email = ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match){
        //发送http验证码
        QJsonObject json_obj;
        json_obj["email"]=email;
       // http://localhost:8080/get_test?key1=value1&key2=value2
        HttpMgr::GetInstance()->PostHttpReq(QUrl("http://localhost:8080/get_varifycode"),
                                            json_obj,ReqId::ID_GET_VARIFY_CODE,Modules::REGISTERMOD);
    }else{
        showTip(tr("邮箱地址不正确"));
    }
}

void RegusterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
        showTip("网络请求错误");
        return;
    }

    //解析JSON 字符串, res 转换为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()){
        showTip("json解析失败");
        return;
    }

    //json 解析错误
    if(!jsonDoc.isObject()){
        showTip("json解析失败");
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    _handlers[id](jsonObj);
    return;

}

void RegusterDialog::initHttpHandles()
{
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"));
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送到邮箱，注意查收"));
        qDebug()<< "email is " << email ;
    });

}

void RegusterDialog::showTip(QString str)
{
    ui->err_trip->setText(str);
    ui->err_trip->setProperty("state","err");
    repolish(ui->err_trip);
}

