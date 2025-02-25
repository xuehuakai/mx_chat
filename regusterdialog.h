#ifndef REGUSTERDIALOG_H
#define REGUSTERDIALOG_H

#include <QDialog>
#include"global.h"
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
private:
    void initHttpHandles();
    Ui::RegusterDialog *ui;
    void showTip(QString);
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;
};

#endif // REGUSTERDIALOG_H
