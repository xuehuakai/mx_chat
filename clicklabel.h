#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QLabel>
#include"global.h"
class ClickLabel : public QLabel
{
    Q_OBJECT
public:
    ClickLabel(QWidget* parent = nullptr);
    virtual void mousePressEvent(QMouseEvent *ev) override;

    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent* event) override;
    void SetState(QString normal="", QString hover="", QString press="",
                  QString select="", QString select_hover="", QString select_press="");
    ClickLbState GetCurState();
protected:
private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;
    QString _selected;
    QString _selected_hover;
    QString _selected_press;
    ClickLbState _curstate;
signals:
    void clicked(void);
};

#endif // CLICKLABEL_H
