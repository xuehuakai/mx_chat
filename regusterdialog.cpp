#include "regusterdialog.h"
#include "ui_regusterdialog.h"
#include"global.h"
#include"httpmgr.h"
#include<QPushButton>
#include<QDebug>
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
    ui->err_trip->clear();

    connect(ui->user_edit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });
    connect(ui->pass_edit, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });
    connect(ui->confirm_edit, &QLineEdit::editingFinished, this, [this](){
        checkConfirmValid();
    });
    connect(ui->verify_edit, &QLineEdit::editingFinished, this, [this](){
        checkVarifyValid();
    });

    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);


    ui->pass_visible->SetState("unvisible","unvisible_hover","","visible",
                               "visible_hover","");
    ui->confirm_visible->SetState("unvisible","unvisible_hover","","visible",
                                  "visible_hover","");
//链接点击事件
    connect(ui->pass_visible,&ClickLabel::clicked,this,[this](){
        ClickLbState state  = ui->pass_visible->GetCurState();
        if(state == ClickLbState::Normal){
            ui->pass_edit->setEchoMode(QLineEdit::Password);
        }else{
            ui->pass_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    connect(ui->confirm_visible, &ClickLabel::clicked, this, [this]() {
        auto state = ui->confirm_visible->GetCurState();
        if(state == ClickLbState::Normal){
            ui->confirm_edit->setEchoMode(QLineEdit::Password);
        }else{
            ui->confirm_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    _countdown_timer = new QTimer(this);

    connect(_countdown_timer, &QTimer::timeout, [this](){
        if(_countdown==0){
            _countdown_timer->stop();
            emit sigSwitchLogin();
            return;
        }
        _countdown--;
        auto str = QString("注册成功，%1 s后返回登录").arg(_countdown);
        ui->tip_lib->setText(str);
    });
}

RegusterDialog::~RegusterDialog()
{
    delete ui;
    qDebug()<<"reg_dlg析构调用";
}

void RegusterDialog::on_get_code_btn_clicked()
{
    qDebug()<<"get varify_code";
    QString email = ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match){
        //发送http验证码
        QJsonObject json_obj;
        json_obj["email"]=email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl("http://127.0.0.1:8080/get_varifycode"),
                                            json_obj,ReqId::ID_GET_VARIFY_CODE,Modules::REGISTERMOD);
    }else{
        showTip(tr("邮箱地址不正确"),false);
    }
}

void RegusterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
        showTip("网络请求错误",false);
        return;
    }

    //解析JSON 字符串, res 转换为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()){
        showTip("json解析失败",false);
        return;
    }

    //json 解析错误
    if(!jsonDoc.isObject()){
        showTip("json解析失败",false);
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

            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送到邮箱，注意查收"),false);
        qDebug()<< "email is " << email ;
    });

    //注册注册用户回包逻辑
    _handlers.insert(ReqId::ID_REG_USER,[this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCCESS){
            if(error == ErrorCodes::UserExist){
                showTip(tr("用户已经存在"),false);
            }else{
                showTip(tr("回包参数错误"),false);
            }
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"),true);
        qDebug()<<"uuid is "<<jsonObj["uuid"].toString();
        qDebug()<<" 注册成功 email is "<<email;

        ChangeTipPage();
    });



}

void RegusterDialog::showTip(QString str,bool b_ok)
{
    if(b_ok){
        ui->err_trip->setProperty("state","normal");
    }else{
        ui->err_trip->setProperty("state","err");
    }

    ui->err_trip->setText(str);

    repolish(ui->err_trip);
}


void RegusterDialog::on_cancel_btn_clicked()
{
    //emit SigCancelBtn();
    _countdown_timer->stop();
    emit sigSwitchLogin();
}


void RegusterDialog::on_sure_btn_clicked()
{
    bool valid = checkUserValid();
    if(!valid){
        return;
    }
    valid = checkEmailValid();
    if(!valid){
        return;
    }
    valid = checkPassValid();
    if(!valid){
        return;
    }
    valid = checkVarifyValid();
    if(!valid){
        return;
    }

    //发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"]=ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = xorString(ui->pass_edit->text());
    json_obj["confirm"] = xorString(ui->confirm_edit->text());
    json_obj["varifycode"] = ui->verify_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl("http://localhost:8080/user_register"),
                                        json_obj,ReqId::ID_REG_USER,Modules::REGISTERMOD);
}

bool RegusterDialog::checkUserValid()
{
    if(ui->user_edit->text() == ""){
        AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegusterDialog::checkConfirmValid()
{
    auto pass = ui->pass_edit->text();
    auto confirm = ui->confirm_edit->text();

    if(pass != confirm){
        AddTipErr(TipErr::TIP_PWD_CONFIRM,tr("确认密码和密码不匹配"));
        return false;
    }else{
        DelTipErr(TipErr::TIP_PWD_CONFIRM);
    }
    return true;
}

void RegusterDialog::ChangeTipPage()
{
    _countdown_timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    // 启动定时器，设置间隔为1000毫秒（1秒）
    _countdown_timer->start(1000);

}
bool RegusterDialog::checkPassValid()
{
    auto pass = ui->pass_edit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}
bool RegusterDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->email_edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}
bool RegusterDialog::checkVarifyValid()
{
    auto pass = ui->verify_edit->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void RegusterDialog::AddTipErr(TipErr te,QString tips){
    _tip_errs[te]=tips;
    showTip(tips,false);
}
void RegusterDialog::DelTipErr(TipErr te){
    _tip_errs.remove(te);
    if(_tip_errs.empty()){
        ui->err_trip->clear();
        return;
    }
    showTip(_tip_errs.first(),false);
}

void RegusterDialog::on_return_btn_clicked()
{
    _countdown_timer->stop();
    emit sigSwitchLogin();
}

