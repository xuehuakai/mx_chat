#ifndef REGUSTERDIALOG_H
#define REGUSTERDIALOG_H

#include <QDialog>
#include"global.h"
#include "ui_regusterdialog.h"
namespace Ui {
class RegusterDialog;
}

class RegusterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegusterDialog(QWidget *parent = nullptr);
    ~RegusterDialog();

private slots:
    void on_get_code_btn_clicked();
    void slot_reg_mod_finish(ReqId id,QString res,ErrorCodes err);
    void on_cancel_btn_clicked();

    void on_sure_btn_clicked();

    void on_return_btn_clicked();

private:
    void initHttpHandles();
    Ui::RegusterDialog *ui;
    void showTip(QString,bool);
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;

    QMap<TipErr,QString> _tip_errs;//输入完成的提示错误，如果该输入框错误清除后就显示剩余的错误，每次显示一条

    void AddTipErr(TipErr te,QString tips);
    void DelTipErr(TipErr te);

    bool checkVarifyValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkUserValid();
    bool checkConfirmValid();

    void ChangeTipPage();

    QTimer * _countdown_timer;
    int _countdown;
signals:
    void SigCancelBtn();
    void sigSwitchLogin();
};

#endif // REGUSTERDIALOG_H
