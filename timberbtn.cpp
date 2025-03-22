#include "timberbtn.h"
#include<QMouseEvent>
TimberBtn::TimberBtn(QWidget *parent) : QPushButton(parent) {
    _counter = 10;
    _timer = new QTimer(this);
    connect(_timer,&QTimer::timeout,[this](){
        _counter--;
        if(_counter<=0){
            _timer->stop();
            _counter = 10;
            this->setText("获取");
            this->setEnabled(true);
            return;
        }
        this->setText(QString::number(_counter));
    });
}

TimberBtn::~TimberBtn()
{
    _timer->stop();

}

void TimberBtn::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        //处理左键释放事件
        this->setEnabled(false);
        this->setText(QString::number(_counter));
        _timer->start(1000);
    }
    QPushButton::mouseReleaseEvent(e);
}
