#ifndef TIMBERBTN_H
#define TIMBERBTN_H

#include <QPushButton>
#include<QTimer>
class TimberBtn : public QPushButton
{
public:
    TimberBtn(QWidget *parent = nullptr);
    ~TimberBtn();
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
private:
    QTimer* _timer;
    int _counter;
};

#endif // TIMBERBTN_H
