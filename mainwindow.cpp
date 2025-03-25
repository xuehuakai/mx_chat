#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"logindialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) //切换到注册后 登录界面可能被删除
{
    ui->setupUi(this);
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);

    //创建和注册消息连接
    connect(_login_dlg,&LoginDialog::SwitchRegister,this,&MainWindow::SlotSwitchReg);
    connect(_login_dlg,&LoginDialog::switchReset,this,&MainWindow::SlotSwitchReset);
}

MainWindow::~MainWindow()
{

    qDebug()<<"MainWindow析构";

    delete ui;
    if(_login_dlg){
        delete _login_dlg;
        _login_dlg = nullptr;
    }

    if(_reg_dlg){
        delete _reg_dlg;
        _reg_dlg = nullptr;
    }
}

void MainWindow::SlotSwitchReg()
{
    _reg_dlg = new RegusterDialog(this);

    _reg_dlg->hide();

    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    //connect(_reg_dlg,&RegusterDialog::SigCancelBtn,this,&MainWindow::SlotCancelReg);
    connect(_reg_dlg,&RegusterDialog::sigSwitchLogin,this,&MainWindow::SlotSwitchLogin);
    _login_dlg->hide();
    setCentralWidget(_reg_dlg);
    _reg_dlg->show();
}

void MainWindow::SlotCancelReg()
{
    qDebug() << "接收取消信号成功";
    _login_dlg = new LoginDialog(this);
    // 隐藏注册对话框
    _reg_dlg->hide();

    // 显示登录对话框并设置为中心部件
    if(_login_dlg){
        _login_dlg->show();
        setCentralWidget(_login_dlg);
    }else{
        qDebug()<<"_login_dlg为空";
    }
}

void MainWindow::SlotSwitchLogin()
{
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);

    _reg_dlg->hide();
    _login_dlg->show();
    //链接登录页面注册信号
    connect(_login_dlg,&LoginDialog::SwitchRegister,this,&MainWindow::SlotSwitchReg);
    //连接登录界面忘记密码信号
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchLogin2()
{
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);

    _reset_dlg->hide();
    _login_dlg->show();
    //链接登录页面注册信号
    connect(_login_dlg,&LoginDialog::SwitchRegister,this,&MainWindow::SlotSwitchReg);
    //连接登录界面忘记密码信号
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset()
{
   //创建一个centralwidget，并将其设置为MainWindow的中心部件
    _reset_dlg = new ResetDialog(this);
   _reset_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_reset_dlg);
   _login_dlg->hide();
    _reset_dlg->show();
   connect(_reset_dlg,&ResetDialog::switchLogin,this,&MainWindow::SlotSwitchLogin2);
}
