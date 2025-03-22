#include"mainwindow.h"

#include <QApplication>
#include<QFile>
#include<QMessageBox>
#include<QDir>
#include<QSettings>
QString gate_url_prefix = "http://localhost:8080";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //解析配置逻辑
    {
        QString app_path = QCoreApplication::applicationDirPath();
        //拼接文件名
        QString fileName = "config.ini";
        QString config_path = QDir::toNativeSeparators(app_path+QDir::separator()+fileName);

        QSettings settings(config_path,QSettings::IniFormat);
        QString gate_host = settings.value("GateServer/host").toString();
        QString gate_port = settings.value("GateServer/port").toString();
        gate_url_prefix = "http://"+gate_host+":"+gate_port;
    }
    qDebug()<<"程序启动"<<"\n";
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
    a.exec();
    return a.exec();
}
