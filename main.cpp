#include "mainwindow.h"

#include <QApplication>
#include<QFile>
#include<QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss(":/style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly)){
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        QMessageBox::warning(nullptr,"错误","打开qss文件失败");
    }
    MainWindow w;
    w.show();
    return a.exec();
}
