#ifndef REGUSTERDIALOG_H
#define REGUSTERDIALOG_H

#include <QDialog>

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

private:
    Ui::RegusterDialog *ui;
    void showTip(QString);
};

#endif // REGUSTERDIALOG_H
