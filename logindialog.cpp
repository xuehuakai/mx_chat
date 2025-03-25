#include "logindialog.h"
#include "ui_logindialog.h"
#include"clicklabel.h"
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    connect(ui->reg_btn,&QPushButton::clicked,this,&LoginDialog::SwitchRegister);
    ui->forget_label->SetState("normal","hover","","selected","selected_hover");
    ui->forget_label->setCursor(Qt::PointingHandCursor);
    connect(ui->forget_label,&ClickLabel::clicked,this,&LoginDialog::slot_forget_pwd);
}

LoginDialog::~LoginDialog()
{
    delete ui;
    qDebug()<<"login_dlg析构调用";
}

void LoginDialog::slot_forget_pwd()
{
    emit switchReset();
}


