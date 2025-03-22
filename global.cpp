#include"global.h"

std::function<void(QWidget*)> repolish = [](QWidget* w){
    w->style()->unpolish(w);
    w->style()->polish(w);
};


std::function<QString(QString)> xorString = [](QString input){
    QString res = input;
    int len = input.length();
    len = len%255;
    for(int i=0;i<len;i++){
        //对每个字符进行异或操作
        //注：这里假设字符都是ASCII,因此直接转换为QChar
        res[i]=QChar(static_cast<ushort>(input[i].unicode() ^ static_cast<ushort>(len)));
    }
    return res;

};
