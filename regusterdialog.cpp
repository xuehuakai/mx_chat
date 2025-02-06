#include "regusterdialog.h"
#include "ui_regusterdialog.h"
#include"global.h"
RegusterDialog::RegusterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegusterDialog)
{
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);
    ui->err_trip->setProperty("state","normal");
    repolish(ui->err_trip);
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
    }else{
        showTip(tr("邮箱地址不正确"));
    }
}

void RegusterDialog::showTip(QString str)
{
    ui->err_trip->setText(str);
    ui->err_trip->setProperty("state","err");
    repolish(ui->err_trip);
}

