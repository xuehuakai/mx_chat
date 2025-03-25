#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"logindialog.h"
#include"regusterdialog.h"
#include"resetdialog.h"
QT_BEGIN_NAMESPACE

/***************************************
* @file mainwindow.h
* @author 墨雪
* @email 2092776196@qq.com
* @date 2025/01/06
* @brief 主窗口
***************************************/

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void SlotSwitchReg();
    void SlotCancelReg();
    void SlotSwitchLogin();
    void SlotSwitchLogin2();
    void SlotSwitchReset();

private:
    Ui::MainWindow *ui;
    LoginDialog * _login_dlg;
    RegusterDialog * _reg_dlg;
    ResetDialog* _reset_dlg;
};
#endif // MAINWINDOW_H
